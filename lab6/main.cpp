#include "stdafx.h"
#include <Windows.h>
#include <clocale>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <list>

using namespace std;

struct AirTicketApp
{
	string destination;		// пункт назначения
	int flightNumber;		// номер рейса
	string fullName;		// фамилия и инициалы
	string departureDate;	// желаемая дата вылета
};

int Menu();

void AddApplication(list<AirTicketApp> *lstApp);		// добавление заявок в список
void RemoveApplications(list<AirTicketApp> *lstApp);	// удаление заявок
void FindApplications(list<AirTicketApp> *lstApp);		// вывод заявок по заданному номеру рейса и дате вылета
void PrintApplications(list<AirTicketApp> *lstApp);		// вывод всех заявок

list<AirTicketApp> LoadApplications(const char *fileName);				// загрузка заявок из файла
void SaveApplications(list<AirTicketApp> lstApp, const char *fileName);	// сохранение заявок в файл

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	const char *fileName = "app_list.txt";
	list<AirTicketApp> appList;
	int d = 0;

	do
	{
		d = Menu();
		switch (d)
		{
		case 1:
			AddApplication(&appList);
			break;
		case 2:
			RemoveApplications(&appList);
			break;
		case 3:
			FindApplications(&appList);
			break;
		case 4:
			PrintApplications(&appList);
			break;
		case 5:
			appList = LoadApplications(fileName);
			break;
		case 6:
			SaveApplications(appList, fileName);
			break;
		case 0:
			cout << "Выход" << endl;
			break;
		default:
			cout << "Выбранный пункт не существует, повторите ввод" << endl;
			break;
		}
		system("Pause");
	} while (d != 0);

	return 0;
}

int Menu()
{
	system("cls");
	cout << "Выберите операцию:" << endl;
	cout << "1. Добавить заявку в список" << endl;
	cout << "2. Удалить заявку" << endl;
	cout << "3. Вывесть заявку по заданному номеру рейса и дате вылета" << endl;
	cout << "4. Вывести все заявки" << endl;
	cout << "5. Загрузить заявки из файла" << endl;
	cout << "6. Сохранить заявки в файл" << endl;
	cout << "0. Выход" << endl;
	cout << "> ";

	int d;
	cin >> d;
	return d;
}

void AddApplication(list<AirTicketApp>* lstApp)
{
	AirTicketApp newApp;

	cout << "Добавление заявки в список" << endl;
	cin.ignore();

	cout << "Введите пункт назначения: ";
	getline(cin, newApp.destination);
	cout << "Введите номер рейса: ";
	cin >> newApp.flightNumber;
	cin.ignore();

	cout << "Введите фамилию и инициалы: ";
	getline(cin, newApp.fullName);
	cout << "Введите желаемую дату вылета: ";
	getline(cin, newApp.departureDate);

	lstApp->push_back(newApp);
	cout << "Заявка добавлена в список" << endl;
}

void RemoveApplications(list<AirTicketApp>* lstApp)
{
	int remove_index;
	cout << "Введите номер удаляемого элемента (нумерация с 1): ";
	cin >> remove_index;

	list<AirTicketApp>::iterator it = lstApp->begin();
	int i = 1;
	while (i != remove_index && it != lstApp->end())
	{
		i++;
		it++;
	}

	if (it == lstApp->end())
	{
		cout << "Элемент с заданным номером не найден" << endl;
		return;
	}

	lstApp->erase(it);
	cout << "Удаление элемента успешно выполнено" << endl;
}

void FindApplications(list<AirTicketApp>* lstApp)
{
	int number;
	string date;

	cout << "Введите значения для поиска:" << endl;
	cout << "Номер рейса: ";
	cin >> number;
	cout << "Дата вылета: ";
	cin >> date;

	list<AirTicketApp> resList;
	list<AirTicketApp>::iterator it = lstApp->begin();
	while (it != lstApp->end())
	{
		if (it->flightNumber == number &&
			it->departureDate == date)
			resList.push_back(*it);
		it++;
	}

	cout << "Рузультаты поиска:" << endl;
	if (resList.size() > 0)
		PrintApplications(&resList);
	else
		cout << "По заданным парамерам заявок не найдено" << endl;
}

void PrintApplications(list<AirTicketApp>* lstApp)
{
	cout << "Список заявок на авиабилеты:" << endl;
	cout << "№ заявки  Пункт назначения  № рейса  Фамилия и инициалы  Желаемая дата вылета" << endl;

	list<AirTicketApp>::iterator it = lstApp->begin();
	int i = 1;
	while (it != lstApp->end())
	{
		cout << setw(8)  << i ++;
		cout << setw(18) << it->destination;
		cout << setw(9)  << it->flightNumber;
		cout << setw(20) << it->fullName;
		cout << setw(22) << it->departureDate;
		cout << endl;
		it++;
	}
	cout << "Итого заявок: " << i - 1 << endl;
}

list<AirTicketApp> LoadApplications(const char * fileName)
{
	list<AirTicketApp> loadedList;
	AirTicketApp app;

	ifstream fin(fileName);
	if (!fin.is_open())
	{
		cout << "Ошибка открытия файла для чтения" << endl;
		return loadedList;
	}

	string str;
	int pos;

	while (!fin.eof())
	{
		getline(fin, str);
		if (str.empty())
			break;

		pos = str.find(";");
		app.destination = str.substr(0, pos);
		str.erase(0, pos + 1);

		pos = str.find(";");
		app.flightNumber = atoi(str.substr(0, pos).c_str());
		str.erase(0, pos + 1);

		pos = str.find(";");
		app.fullName = str.substr(0, pos);
		str.erase(0, pos + 1);

		app.departureDate = str;
		loadedList.push_back(app);
	}

	cout << "Загружено заявок: " << loadedList.size() << endl;
	return loadedList;
}

void SaveApplications(list<AirTicketApp> lstApp, const char * fileName)
{
	ofstream fout(fileName);
	if (!fout.is_open())
	{
		cout << "Ошибка открытия файла для записи" << endl;
		return;
	}

	list<AirTicketApp>::iterator it = lstApp.begin();
	int i = 1;
	while (it != lstApp.end())
	{
		fout << it->destination << ";";
		fout << it->flightNumber << ";";
		fout << it->fullName << ";";
		fout << it->departureDate << endl;
		i++;
		it++;
	}

	fout.close();
	cout << "Сохранено заявок: " << i - 1 << endl;
}
