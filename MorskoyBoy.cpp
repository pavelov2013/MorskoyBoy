#include <iostream>
#include <windows.h>
#include<functional>
#include <vector>
#include <ctime>
#include<thread>
#include <chrono>
#define ship '#'
#define destroyed 'X'

const char chars[] = {'A','B','C','D','E','F','G','H','I','J'};
void DrawArea()
{
	std::cout << '\0';
	for (int i = 0; i < sizeof(chars); i++) std::cout << '\0' << chars[i];
	for (int i = 0; i < 21; i++) std::cout << '\0';
	for (int i = 0; i < sizeof(chars); i++) std::cout << '\0' << chars[i];
	for (int i = 0; i < sizeof(chars)-1; i++)
	{
		std::cout << std::endl <<'\0'<< i + 1;
		for (int z = 0; z < 40; z++) std::cout << '\0';
		std::cout << i + 1;
	}
	std::cout << std::endl << "10";
	for (int z = 0; z < 39; z++) std::cout << '\0';
	std::cout << "10";
	std::cout << std::endl;
}
void SetCursor(const int& x, const int& y)
{
	COORD crd;
	crd.X = x;
	crd.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), crd);
}
int getXcoord()
{
	CONSOLE_SCREEN_BUFFER_INFO info_x;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info_x);
	return info_x.dwCursorPosition.X;
}

int getYcoord()
{
	CONSOLE_SCREEN_BUFFER_INFO info_y;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info_y);
	return info_y.dwCursorPosition.Y;
}

int Index(const char arr[], const int& symbol)
{
	for (int i = 0; i <= sizeof(arr); i++)
	{
		if ((int)arr[i] == symbol) return i;
	}
}
std::vector<int*> Sort(const std::vector<int*>& arr,bool mode)
{
		std::vector<int*> coords = arr;
		bool iter = true;
		while (iter)
		{
			iter = false;
			for (int i = 0; i < coords.size() - 1; i++)
			{
				if (coords[i][mode] > coords[i + 1][mode])
				{
					int last = coords[i][mode];
					coords[i][mode] = coords[i + 1][mode];
					coords[i + 1][mode] = last;
					iter = true;
				}
			}
		}
		return coords;

}
void ReDraw();
class Ships
{
public:
	int size;
	std::vector<int*> coords;
	bool* alive;
	bool mode;
	Ships( const int& size,bool mode)
	{
		alive = new bool[size];
		for (int j = 0; j < this->size; j++) *alive = true;
		this->size = size;
		this->mode = mode;
		for (int i = 0; i < this->size; i++) //Упорядочивание координат
		{
			if (i >= 3)
			{
				if (this->coords[0][0] == this->coords[1][0]) this->coords = Sort(coords, 1);
				else if(this->coords[0][1] == this->coords[1][1]) this->coords = Sort(coords, 0);
			}
			this->coords.push_back((this->Create(i, this->mode)));
		}
		Print();
	}
	~Ships()
	{
		for (int i = 0; i < this->size; i++) delete[] coords[i];
	}
	void Print()
	{
		for (int i = 0; i < this->size; i++)
		{
			this->mode  ? SetCursor(2 * Index(chars, (this->coords[i][0]))+43, this->coords[i][1]) : SetCursor(2 * Index(chars, (this->coords[i][0]))+2, this->coords[i][1]);
			if (alive[i])
			{
				if (mode == 0) std::cout << ship;
			}
			else std::cout << destroyed;
		} 
		SetCursor(0,11);
	}
	int* Create(const int& i,const bool mode)
	{

		int* rt = new int[2];
		if (mode == 0)
		{
			char crds[256];
			do
			{
				std::cout << i + 1 << "-е координаты " << this->size << " палубного корабля: ";
				std::cin >> crds;
				std::cout << std::endl;
			} while ((!((crds[0] >= 65 && crds[1] <= 74) && (atoi(&crds[1]) >= 1 && atoi(&crds[1]) <= 10) && CheckCoords(0,(int)crds[0],atoi(&crds[1]),i))));

			rt[0] = crds[0];
			rt[1] = atoi(&crds[1]);
		}
		else
		{
			do
			{
				rt[1] = rand() % 10 + 1;
				rt[0] = rand() % 10 + 65;
			}
			while ((!( CheckCoords(1, rt[0],rt[1],i))));
		}
		return rt;
		delete[] rt;
	}
	bool CheckCoords(const bool mode,int x, const int& y,int j);
	bool Destroy(int i)
	{
		alive[i] = false;
		int cursor_x = getXcoord();
		int cursor_y = getYcoord();
		this->mode ? SetCursor(2 * Index(chars, (this->coords[i][0])) + 43, this->coords[i][1]) : SetCursor(2 * Index(chars, (this->coords[i][0])) + 2, this->coords[i][1]);
		std::cout << destroyed;
		SetCursor(cursor_x,cursor_y);
		//0 - ранил 1 - убил
		for (int t = 0; t < size; t++) if (alive[t]) return false;
		return true;
	}
};
std::vector<Ships*> YourShips;
std::vector<Ships*> EnemyShips;
void ReDraw()
{
	system("cls");
	DrawArea();
	for (int i = 0; i < YourShips.size(); i++) YourShips[i]->Print();
}
bool Ships::CheckCoords(const bool mode,int x, const int& y,int j)
{
	std::vector<Ships*>* p = new std::vector<Ships*>();
	switch (mode)
	{
	case 0: {p = &YourShips; break;}
	case 1: {p = &EnemyShips; break; }
	}
	for (int i = 0; i < p->size(); i++) // проверка на совпадение координат с другими кораблями
	{
		for (int z = 0; z < (*p)[i]->coords.size(); z++)
		{
			if (x == (*p)[i]->coords[z][0] && y == (*p)[i]->coords[z][1])
			{ 
				p = nullptr;
				return false; 
			}
		}
	}
	// проверка на совпадение с локальными координатами
	if (this->coords.size() > 0)
		for (int i = 0; i < this->coords.size(); i++)
			if (this->coords[i][0] == x && this->coords[i][1] == y) return false;
	//проверка на  размещение в ряд
	if (j >= 1 && !((abs(x - coords[j - 1][0]) == 1 && (y == coords[j - 1][1])) || (abs(y - coords[j - 1][1]) == 1 && (x == coords[j - 1][0])) || (abs(x - coords[0][0]) == 1 && (y == coords[0][1])) || (abs(y - coords[0][1]) == 1 && (x == coords[0][0])))) return false;
	if (j >= 2 && !((x - (coords[j - 1])[0] == (coords[j - 1])[0] - (coords[j - 2])[0] && y - (coords[j - 1])[1] == (coords[j - 1])[1] - (coords[j - 2])[1]) || (x - (coords[0])[0] == (coords[0])[0] - (coords[1])[0] && y - (coords[0])[1] == (coords[0])[1] - (coords[1])[1])))
		return false;
	//Проверка на касание сторон и углов с другими кораблями
	for (int i = 0; i < p->size(); i++)
	{
		for (int z = 0; z < (*p)[i]->coords.size(); z++)
			if ((abs(y - (*p)[i]->coords[z][1]) == 1 && (abs(x - (*p)[i]->coords[z][0]) == 1 || abs(x - (*p)[i]->coords[z][0]) == 0)) || (abs(x - (*p)[i]->coords[z][0]) == 1 && (abs(y - (*p)[i]->coords[z][1]) == 1 || abs(y - (*p)[i]->coords[z][1]) == 0)))
				return false;
	}
	p = nullptr;
	//проверка бота на возможность создания корабля
	if (mode && j==0 && coords.size()>1) 
	{
		bool isTrue = true;
		auto check = [&isTrue](int count, int x_step, int y_step, int x, int y)
		{
			for (int k = 0; k < count; k++)
			{
				for (int i = 0; i < EnemyShips.size(); i++)
				{
					for (int z = 0; z < count; z++)
						if (!(x + x_step * k == EnemyShips[i]->coords[z][0] && y + y_step * k == EnemyShips[i]->coords[z][0])) { isTrue = false; return false; };
				}
			}
		};

		int count = this->coords.size();

		int x_step = 1;
		int y_step = 0;
		check(count, x_step, y_step, x, y);

		 x_step = -1;
		 y_step = 0;
		 check(count, x_step, y_step, x, y);

		 x_step = 0;
		 y_step = 1;
		 check(count, x_step, y_step, x, y);
		
		 x_step = 0;
		 y_step = -1;
		 check(count, x_step, y_step, x, y);
	}
	return true;
}
int Hit(int x,int y, std::vector<Ships*>& enemy) //0 - мимо 1 - ранил 2-убил
{
	for(int i =0;i<enemy.size();i++)
		for(int z=0;z<enemy[i]->coords.size();z++)
			if (x == enemy[i]->coords[z][0] && y == enemy[i]->coords[z][1])
			{
				int result = (*enemy[i]).Destroy(z);
				if (result) { enemy.erase(enemy.begin() + i); return 2; }
				else return 1;
			}
	return 0;
}
int main()
{
	setlocale(0, "ru");
	DrawArea();
	//enemy
	srand(time(NULL));
	for (int i = 4; i >= 1; i--)
	{
		for (int z = 5 - i; z >= 1; z--)
		{
			Ships* s = new Ships(i, 1);
			EnemyShips.push_back(s);
		}
	}
	//Your
	std::cout << "Вы расставляете корабли..." << std::endl;
	for (int i = 1; i <= 4; i++)
	{
		for (int z = 5 - i; z >= 1; z--)
		{
			Ships* s = new Ships(i,0);
			YourShips.push_back(s);
		}
	}
	SetCursor(0,11);
	std::cout << "Ход игры" << std::endl;
	bool who = 0; //0 - you 1- enemy
	
		while (EnemyShips.size() != 0 && YourShips.size() != 0)
		if (who == 0) // you
		{
			std::cout << "Ваш ход: ";
			char crds[256];
			do std::cin >> crds; while (!(crds[0] >= 65 && crds[1] <= 74) && (atoi(&crds[1]) >= 1 && atoi(&crds[1]) <= 10));
			int rt[2];
			rt[0] = crds[0];
			rt[1] = atoi(&crds[1]);
			std::cout << (char)rt[0] << rt[1];
			//Проверка удара
			switch (Hit(rt[0], rt[1], EnemyShips))
			{
			case 0: {std::cout << "Мимо!"; who = 1; break; }
			case 1: {std::cout << "Ранил!"; break; }
			case 2: {std::cout << "Убил!"; break; }
			}
			std::cout << std::endl;
		}
		else
		{
			std::cout << "Ход противника: ";
			int rt[2];
			rt[1] = rand() % 10 + 1;
			rt[0] = rand() % 10 + 65;
			std::cout << (char)rt[0] << rt[1];
			switch (Hit(rt[0], rt[1], YourShips))
			{
			case 0: {std::cout << "Мимо!"; who = 0; break; }
			case 1: {std::cout << "Ранил!"; break; }
			case 2: {std::cout << "Убил!"; break; }
			}
			std::cout << std::endl;
		}
		if (EnemyShips.size() == 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			system("cls");
			std::cout << "Вы выиграли!!!" << std::endl;
		}
		if (YourShips.size() == 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			system("cls");
			std::cout << "Вы проиграли(((" << std::endl;
		}
	return 0;
}