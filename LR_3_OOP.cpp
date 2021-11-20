#include <iostream>
#include <vector>
#include <Windows.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>

using namespace std;

class Date
{
public:
	int day;
	int month;
	int year;
	Date(int d_temp, int m_temp, int y_temp);

};

Date::Date(int d_temp, int m_temp, int y_temp)
{
	day = d_temp;
	month = m_temp;
	year = y_temp;

}

class Storage 
{
public:
	string product_name;
	float price;
	int amount;
	int section;
	Storage(string pname, float price_temp, int amount_temp, int sect_temp);

};

Storage::Storage(string pname, float price_temp, int amount_temp, int sect_temp)
{
	product_name = pname;
	price = price_temp;
	amount = amount_temp;
	section = sect_temp;
}

class Database
{
public:
	vector<Storage> storage;
	vector<Date> date;

	const string filepath = "db.txt";
	bool exists;

	void add();
	void print();
	void sort_amount_decrease();
	void search_by_section();
	void search_over_date();
	int Julian(Date);

	bool read();
	void write();

	Database();
};

Database::Database()
{
	exists = false;
}

void Database::add()
{
	string pname, empty;
	float price_temp;
	int amount_temp;
	int sect_temp;
	int d_temp;
	int m_temp;
	int y_temp;

	cout << "Введите название товара: ";
	getline(cin, empty); // баг с чтением пустой строки
	getline(cin, pname);

	cout << "Введине цену товара: ";
	cin >> price_temp;

	cout << "Введите количество товара: ";
	cin >> amount_temp;

	cout << "Введите номер секции: ";
	cin >> sect_temp;
	
	cout << "Введите день: ";
	cin >> d_temp;

	cout << "Введите месяц: ";
	cin >> m_temp;

	cout << "Введите год: ";
	cin >> y_temp;

	storage.emplace_back(pname, price_temp, amount_temp, sect_temp);
	date.emplace_back(d_temp, m_temp, y_temp);
}

void Title()
{
	cout << endl;
	cout << "№";
	cout.width(15);
	cout << "Название товара";
	cout.width(20);
	cout << "Цена товара";
	cout.width(20);
	cout << "Количество товара";
	cout.width(20);
	cout << "Номер секции";
	cout.width(20);
	cout << "Срок годности" << endl;
}

void print_product(Storage temp, Date dtemp)
{
	cout.width(15);
	cout << temp.product_name;
	cout.width(20);
	cout << temp.price;
	cout.width(20);
	cout << temp.amount;
	cout.width(20);
	cout << temp.section;
	cout.width(13);
	cout << dtemp.day << "/" << dtemp.month << "/" << dtemp.year << endl;
}

void Database::print()
{
	Title();
	for (int i = 0; i < storage.size(); i++)
	{
		Storage temp = storage[i];
		Date dtemp = date[i];
		
		cout << i + 1;
		print_product(temp, dtemp);
	}
}

void Database::sort_amount_decrease()
{
	Title();

	vector<Storage> temp = storage;
	vector<Date> dtemp = date;

	struct
	{
		bool operator()(const Storage s1, const Storage s2)
		{
			return s1.amount < s2.amount;
		}
	}comp;
	
	sort(temp.begin(), temp.end(), comp);

	for (int i = 0; i < storage.size(); i++)
	{
		Storage s = temp[i];
		Date d = dtemp[i];
		cout << i + 1;
		print_product(s, d);
	}
	
}

void Database::search_by_section()
{
	int search;
	bool title_print = false;

	cout << "Введите номер секции для поиска: ";
	cin >> search;
	
	for (int i = 0; i < storage.size(); i++)
	{
		Storage temp = storage[i];
		Date dtemp = date[i];
		if (temp.section == search)
		{
			if (!title_print)
			{
				title_print = true;
				Title();
			}
			cout << i + 1;
			print_product(temp, dtemp);
		}
		else
		{
			cout << "Совпадений не найдено";
		}
	}
}

int Database::Julian(Date dtemp)
{
	int a = (14 - dtemp.month) / 12;  										//Промежуточные коэффициенты
	int y = dtemp.year + 4800 - a;
	int m = dtemp.month + 12 * a - 3;
	int JDN = dtemp.day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045; 		//Переход к Юлианскому дню
	return JDN;
}

void Database::search_over_date()
{
	vector<Storage> upd_storage;
	int d, m, y;
	bool title_print = false;

	cout << endl << "Введите день: ";
	cin >> d;
	cout << "Введите месяц: ";
	cin >> m;
	cout << "Введите год: ";
	cin >> y;

	Date dtemp(d, m, y);
	int search = Julian(dtemp);

	for (int i = 0; i < storage.size(); i++)
	{
		Storage temp = storage[i];
		Date dtemp2 = date[i];
		int j_dt2 = Julian(dtemp2);
		if (j_dt2 > search)
		{
			Title();
			title_print = true;
			cout << i + 1;
			print_product(temp, dtemp2);
			upd_storage.push_back(temp);
		}
	}

	swap(storage, upd_storage);
}

bool Database::read()
{
	ifstream input_stream(filepath);

	if (input_stream.fail())
	{
		exists = false;
		return false;
	}

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	string pname;
	float price_temp;
	int amount_temp;
	int sect_temp;
	int d_temp;
	int m_temp;
	int y_temp;

	while (!input_stream.eof() && !input_stream.fail())
	{
		getline(input_stream, pname);
		if (pname.size() == 0) // Баг с чтением пустой строки
		{
			continue;
		}
		input_stream >> price_temp;
		input_stream >> amount_temp;
		input_stream >> sect_temp;
		input_stream >> d_temp;
		input_stream >> m_temp;
		input_stream >> y_temp;
		if (pname.size())
		{
			storage.emplace_back(pname, price_temp, amount_temp, sect_temp);
			date.emplace_back(d_temp, m_temp, y_temp);
		}
	}

	return true;
}

void Database::write()
{
	ofstream output_stream(filepath);
	for (int i = 0; i < storage.size(); i++)
	{
		Storage temp = storage[i];
		Date dtemp = date[i];

		output_stream << temp.product_name << "\n"
			<< temp.price << "\n"
			<< temp.amount << "\n"
			<< temp.section << "\n"
			<< dtemp.day << "\n"
			<< dtemp.month << "\n"
			<< dtemp.year << "\n";
	}
	output_stream.close();
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Database db;

	if (!db.read())
	{
		cout << "Файл с БД отсутствует" << endl;
	}
	else
	{
		cout << "Файл с БД найден" << endl;
	}

	int ch = 1;

	while (ch != 0)
	{
		int cmd;

		cout << endl << "Доступные операции" << endl;
		cout << "Вывод в виде таблицы = 1";
		cout.width(50);
		cout << "Добавить новый элемент = 2" << endl;
		cout << "Для поиска по номеру секции = 3"; 
		cout.width(60);
		cout << "Для сортировки по уменьшению количества = 4" << endl;
		cout << "Для отбора по дате = 5" << endl;
		cout << "Завершить процесс и сохранить файл = 0" << endl;
		cout << endl <<"Введите номер команды: ";

		cin >> cmd;
		if (cmd < 0)
		{
			cout << endl << "Ошибка. Отрицательный ввод" << endl;
			continue;
		}
		switch (cmd)
		{
		case 1:
		{
			db.print();
			break;
		}
		case 2:
		{
			db.add();
			break;
		}
		case 3:
		{
			db.search_by_section();
			break;
		}
		case 4:
		{
			db.sort_amount_decrease();
			break;
		}
		case 5:
		{
			db.search_over_date();
			break;
		}
		case 0:
		{
			db.write();
			ch = 0;
			cout << "Файл записан" << endl;
			break;
		}
		default:
		{
			cout << "Введена неверная команда" << endl;
			break;
		}
		}
	}

	cout << "Завершение процесса" << endl;

	return 0;
}