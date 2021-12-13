
// InstalledSoftwaresDlg.h : header file
//

#pragma once

#include <unordered_map>
#include <string>
#if !defined(_UNICODE) && !defined(_UNICODE)
#define TSTRING std::string
#else
#define TSTRING std::wstring
#endif // !defined(_UNICODE) && !defined(_UNICODE)

// CInstalledSoftwaresDlg dialog
class CInstalledSoftwaresDlg : public CDialogEx
{
// Construction
public:
	CInstalledSoftwaresDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSTALLEDSOFTWARES_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	std::unordered_map<TSTRING, TSTRING> m_vInstalledList;
	std::unordered_map<TSTRING, TSTRING> m_vOnceList;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
private:
	BOOL AllRegKeyListInstalled(HKEY hKey, LPCTSTR szDesKeyItem)
	{
		HKEY hSubKey = NULL;
		DWORD dwIndex = 0;
		LSTATUS lResult;

		lResult = RegOpenKeyEx(hKey, szDesKeyItem, 0, KEY_READ, &hSubKey);
		if (ERROR_SUCCESS == lResult)
		{
			DWORD dwRegTypeSZ = REG_SZ;
			TCHAR tzPackageCode[MAX_PATH] = { 0 };
			DWORD cbPackageCode = sizeof(tzPackageCode);
			TCHAR tzProductName[MAX_PATH] = { 0 };
			DWORD cbProductName = sizeof(tzProductName);

			lResult = RegEnumKeyEx(hSubKey, dwIndex, tzPackageCode, &cbPackageCode, 0, NULL, NULL, NULL);
			while (lResult == ERROR_SUCCESS && lResult != ERROR_MORE_DATA)
			{
				HKEY hSubKeyItem = NULL;
				if (::RegOpenKeyEx(hSubKey, tzPackageCode, 0, KEY_READ, &hSubKeyItem) == ERROR_SUCCESS)
				{
					memset(tzProductName, 0, sizeof(tzProductName));
					cbProductName = sizeof(tzProductName);
					(::RegQueryValueEx(hSubKeyItem, TEXT("ProductName"), 0, &dwRegTypeSZ, (LPBYTE)tzProductName, &cbProductName) == ERROR_SUCCESS);
					if (lstrlen(tzProductName) > 0)
					{
						m_vInstalledList.emplace(tzProductName, tzProductName);
					}
					::RegCloseKey(hSubKeyItem);
				}

				dwIndex = dwIndex + 1;
				cbPackageCode = sizeof(tzPackageCode);
				lResult = RegEnumKeyEx(hSubKey, dwIndex, tzPackageCode, &cbPackageCode, 0, NULL, NULL, NULL);
			}
			RegCloseKey(hSubKey);
			return TRUE;
		}
		RegCloseKey(hSubKey);
		return FALSE;
	}
	BOOL AllRegKeyListUninstall(CListCtrl* pListCtrl, HKEY hKey, LPCTSTR szDesKeyItem)
	{
		HKEY hSubKey = NULL;
		DWORD dwIndex = 0;
		LSTATUS lResult = NULL;

		lResult = RegOpenKeyEx(hKey, szDesKeyItem, 0, KEY_READ, &hSubKey);		// ´ò¿ª×¢²á±í
		if (ERROR_SUCCESS == lResult)
		{
			int nId = 0;
			TCHAR tzName[MAX_PATH] = { 0 };
			TCHAR tzDisplayName[MAX_PATH] = { 0 };
			TCHAR tzPublisher[MAX_PATH] = { 0 };
			TCHAR tzInstallDate[MAX_PATH] = { 0 };
			DWORD dwEstimatedSize = 0;
			TCHAR tzDisplayVersion[MAX_PATH] = { 0 };
			DWORD dwSystemComponent = 0;
			DWORD cbName = sizeof(tzName);
			DWORD cbDisplayName = sizeof(tzDisplayName);
			DWORD cbPublisher = sizeof(tzPublisher);
			DWORD cbInstallDate = sizeof(tzInstallDate);
			DWORD cbEstimatedSize = sizeof(dwEstimatedSize);
			DWORD cbDisplayVersion = sizeof(tzDisplayVersion);
			DWORD cbSystemComponent = sizeof(dwSystemComponent);

			DWORD dwRegTypeSZ = REG_SZ;
			DWORD dwRegTypeDWORD = REG_DWORD;

			lResult = RegEnumKeyEx(hSubKey, dwIndex, tzName, &cbName, 0, NULL, NULL, NULL);
			while (lResult == ERROR_SUCCESS && lResult != ERROR_MORE_DATA)
			{
				HKEY hSubKeyItem = NULL;
				if (::RegOpenKeyEx(hSubKey, tzName, 0, KEY_READ, &hSubKeyItem) == ERROR_SUCCESS)
				{
					memset(tzDisplayName, 0, sizeof(tzDisplayName));
					memset(tzPublisher, 0, sizeof(tzPublisher));
					memset(tzInstallDate, 0, sizeof(tzInstallDate));
					memset(&dwEstimatedSize, 0, sizeof(dwEstimatedSize));
					memset(tzDisplayVersion, 0, sizeof(tzDisplayVersion));
					memset(&dwSystemComponent, 0, sizeof(dwSystemComponent));
					cbDisplayName = sizeof(tzDisplayName);
					cbPublisher = sizeof(tzPublisher);
					cbInstallDate = sizeof(tzInstallDate);
					cbEstimatedSize = sizeof(dwEstimatedSize);
					cbDisplayVersion = sizeof(tzDisplayVersion);
					cbSystemComponent = sizeof(dwSystemComponent);
					(::RegQueryValueEx(hSubKeyItem, TEXT("DisplayName"), 0, &dwRegTypeSZ, (LPBYTE)tzDisplayName, &cbDisplayName) == ERROR_SUCCESS);
					(::RegQueryValueEx(hSubKeyItem, TEXT("Publisher"), 0, &dwRegTypeSZ, (LPBYTE)tzPublisher, &cbPublisher) == ERROR_SUCCESS);
					(::RegQueryValueEx(hSubKeyItem, TEXT("InstallDate"), 0, &dwRegTypeSZ, (LPBYTE)tzInstallDate, &cbInstallDate) == ERROR_SUCCESS);
					(::RegQueryValueEx(hSubKeyItem, TEXT("EstimatedSize"), 0, &dwRegTypeDWORD, (LPBYTE)&dwEstimatedSize, &cbEstimatedSize) == ERROR_SUCCESS);
					(::RegQueryValueEx(hSubKeyItem, TEXT("DisplayVersion"), 0, &dwRegTypeSZ, (LPBYTE)tzDisplayVersion, &cbDisplayVersion) == ERROR_SUCCESS);
					(::RegQueryValueEx(hSubKeyItem, TEXT("SystemComponent"), 0, &dwRegTypeDWORD, (LPBYTE)&dwSystemComponent, &cbSystemComponent) == ERROR_SUCCESS);

					if (lstrlen(tzDisplayName) > 0)
					{
						if(dwSystemComponent != 1)
						{
							int nColumnIdx = 0;
							TCHAR tId[MAX_PATH] = { 0 };
							LVFINDINFO lvfi = { 0 };
							lvfi.flags = LVFI_PARTIAL | LVFI_STRING;
							lvfi.psz = (LPCTSTR)tzName;
							nId = pListCtrl->FindItem(&lvfi);
							if (nId == (-1))
							{
								nId = pListCtrl->InsertItem(pListCtrl->GetItemCount(), tId);
							}

							pListCtrl->SetItemText(nId, nColumnIdx, tzName);

							_sntprintf_s(tId, sizeof(tId)/sizeof(*tId), _T("%d"), nId);
							nColumnIdx++;
							pListCtrl->SetItemText(nId, nColumnIdx, tId);

							nColumnIdx++;
							if (lstrlen(tzDisplayName) > 0)
							{
								pListCtrl->SetItemText(nId, nColumnIdx, tzDisplayName);
							}
							nColumnIdx++;
							if (lstrlen(tzPublisher) > 0)
							{
								pListCtrl->SetItemText(nId, nColumnIdx, tzPublisher);
							}
							nColumnIdx++;
							if (lstrlen(tzInstallDate) > 0)
							{
								pListCtrl->SetItemText(nId, nColumnIdx, tzInstallDate);
							}
							nColumnIdx++;
							if (dwEstimatedSize >= 0)
							{
								double d_Size = dwEstimatedSize / 1024.0;
								CString p_Size;
								p_Size.Format(_T("%.2lf M"), d_Size);

								pListCtrl->SetItemText(nId, nColumnIdx, p_Size);
							}
							nColumnIdx++;
							if (lstrlen(tzDisplayVersion) > 0)
							{
								pListCtrl->SetItemText(nId, nColumnIdx, tzDisplayVersion);
							}
						}
					}
					::RegCloseKey(hSubKeyItem);
				}

				dwIndex = dwIndex + 1;
				cbName = sizeof(tzName);
				lResult = RegEnumKeyEx(hSubKey, dwIndex, tzName, &cbName, 0, NULL, NULL, NULL);
			}
			RegCloseKey(hSubKey);
			return TRUE;
		}
		RegCloseKey(hSubKey);
		return FALSE;
	}
};
