#include "PasswordManager.h"

void LoadPasswordsFromFile(const std::wstring& filePath) {
	try {
		std::ifstream file(filePath);
		if (!file.is_open()) {
			MessageBox(NULL, L"�޷����ļ�!", L"����", MB_ICONERROR);
			return;
		}

		json j;
		file >> j;

		passwordEntries.clear();
		ListView_DeleteAllItems(hListView);
		// ����Ӳ����Կ
		std::wstring hardwareKey = GenerateHardwareKey();

		if (j.is_array()) {
			for (auto& item : j) {
				PasswordEntry entry;
				entry.id = Widen(item["id"].get<std::string>());
				entry.website = Widen(item["website"].get<std::string>());
				entry.username = Widen(item["username"].get<std::string>());
				entry.password = AES::DecryptString(Widen(item["password"].get<std::string>()), hardwareKey);
				entry.createdDate = Widen(item["createdDate"].get<std::string>());

				passwordEntries.push_back(entry);

				// ��ӵ�ListView
				LVITEM lvItem;
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = ListView_GetItemCount(hListView);
				lvItem.iSubItem = 0;
				lvItem.pszText = const_cast<wchar_t*>(entry.website.c_str());
				ListView_InsertItem(hListView, &lvItem);

				ListView_SetItemText(hListView, lvItem.iItem, 1, const_cast<wchar_t*>(entry.username.c_str()));
				ListView_SetItemText(hListView, lvItem.iItem, 2, const_cast<wchar_t*>(entry.createdDate.c_str()));
			}
		}

		UpdateStatusBar(L"�Ѽ��������: " + filePath);
	}
	catch (const std::exception& e) {
		std::wstring errorMsg = L"JSON��������: ";
		errorMsg += std::wstring(e.what(), e.what() + strlen(e.what()));
		MessageBox(NULL, errorMsg.c_str(), L"����", MB_ICONERROR);
	}
}

void ImportPasswordsFromFile(const std::wstring& filePath) {
	try {
		std::ifstream file(filePath);
		if (!file.is_open()) {
			MessageBox(NULL, L"�޷����ļ�!", L"����", MB_ICONERROR);
			return;
		}

		json j;
		file >> j;

		if (j.is_array()) {
			for (auto& item : j) {
				PasswordEntry entry;
				entry.id = Widen(item["id"].get<std::string>());
				entry.website = Widen(item["website"].get<std::string>());
				entry.username = Widen(item["username"].get<std::string>());
				entry.password = Widen(item["password"].get<std::string>());
				entry.createdDate = Widen(item["createdDate"].get<std::string>());

				passwordEntries.push_back(entry);

				// ��ӵ�ListView
				LVITEM lvItem;
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = ListView_GetItemCount(hListView);
				lvItem.iSubItem = 0;
				lvItem.pszText = const_cast<wchar_t*>(entry.website.c_str());
				ListView_InsertItem(hListView, &lvItem);

				ListView_SetItemText(hListView, lvItem.iItem, 1, const_cast<wchar_t*>(entry.username.c_str()));
				ListView_SetItemText(hListView, lvItem.iItem, 2, const_cast<wchar_t*>(entry.createdDate.c_str()));
			}
		}

		UpdateStatusBar(L"�Ѽ��������: " + filePath);
	}
	catch (const std::exception& e) {
		std::wstring errorMsg = L"JSON��������: ";
		errorMsg += std::wstring(e.what(), e.what() + strlen(e.what()));
		MessageBox(NULL, errorMsg.c_str(), L"����", MB_ICONERROR);
	}
}

void SavePasswordsToFile(const std::wstring& filePath) {
	try {
		json j = json::array();

		// ����Ӳ����Կ
		std::wstring hardwareKey = GenerateHardwareKey();

		for (const auto& entry : passwordEntries) {
			json item;
			item["id"] = Narrow(entry.id);
			item["website"] = Narrow(entry.website);
			item["username"] = Narrow(entry.username);
			//item["password"] = Narrow(entry.password);
			item["password"] = Narrow(AES::EncryptString(entry.password, hardwareKey));
			item["createdDate"] = Narrow(entry.createdDate);

			j.push_back(item);
		}

		std::ofstream file(filePath);
		if (!file.is_open()) {
			MessageBox(NULL, L"�޷������ļ�!", L"����", MB_ICONERROR);
			return;
		}

		file << j.dump(4);
	}
	catch (const std::exception& e) {
		std::wstring errorMsg = L"�����ļ�����: ";
		errorMsg += std::wstring(e.what(), e.what() + strlen(e.what()));
		MessageBox(NULL, errorMsg.c_str(), L"����", MB_ICONERROR);
	}
}

void ExportPasswordsToFile(const std::wstring& filePath) {
	try {
		json j = json::array();

		// ����Ӳ����Կ
		std::wstring hardwareKey = GenerateHardwareKey();

		for (const auto& entry : passwordEntries) {
			json item;
			item["id"] = Narrow(entry.id);
			item["website"] = Narrow(entry.website);
			item["username"] = Narrow(entry.username);
			item["password"] = Narrow(entry.password);
			item["createdDate"] = Narrow(entry.createdDate);

			j.push_back(item);
		}

		std::ofstream file(filePath);
		if (!file.is_open()) {
			MessageBox(NULL, L"�޷������ļ�!", L"����", MB_ICONERROR);
			return;
		}

		file << j.dump(4);
	}
	catch (const std::exception& e) {
		std::wstring errorMsg = L"�����ļ�����: ";
		errorMsg += std::wstring(e.what(), e.what() + strlen(e.what()));
		MessageBox(NULL, errorMsg.c_str(), L"����", MB_ICONERROR);
	}
}
