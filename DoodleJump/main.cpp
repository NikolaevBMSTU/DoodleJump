#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "resource.h"
#include <math.h>
#include <string.h>
#include <time.h>
using namespace sf;

struct point
{
	int x, y;
};

Image LoadResourcePicturePNG(int PictureResourceCode)
{
	HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(PictureResourceCode), "PNG");
	HGLOBAL hData = 0;
	LPVOID pData;

	DWORD dwSize = SizeofResource(NULL, hRes);
	hData = LoadResource(NULL, hRes);
	pData = LockResource(hData);

	Image ReturnImage;
	ReturnImage.loadFromMemory(pData, dwSize);
	return ReturnImage;
}

Font LoadResourceFont(int FontResourceCode)
{
	HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(FontResourceCode), RT_FONT);
	HGLOBAL hData = 0;
	LPVOID pData;

	DWORD dwSize = SizeofResource(NULL, hRes);
	hData = LoadResource(NULL, hRes);
	pData = LockResource(hData);

	Font ReturnFont;
	ReturnFont.loadFromMemory(pData, dwSize);
	return ReturnFont;
}

int main()
{
	srand(time(0));

	RenderWindow app(VideoMode(400, 533), "Doodle Game!");
	app.setFramerateLimit(60);

	// �������� �������
	Texture TextureBackground, TextureRegularPlat, TextureDoodleR, TextureDoodleL;
	TextureBackground.loadFromImage(LoadResourcePicturePNG(IDB_PNG1));
	TextureDoodleR.loadFromImage(LoadResourcePicturePNG(IDB_PNG2));
	TextureDoodleL.loadFromImage(LoadResourcePicturePNG(IDB_PNG3));
	TextureRegularPlat.loadFromImage(LoadResourcePicturePNG(IDB_PNG4));

	// �������� ����������� ������
	Font FontDimboRegular = LoadResourceFont(IDR_FONT1);

	// ��������� ������� "Game Over"
	Text GameOverText("              Game Over\n Press Space to continue...", FontDimboRegular);
	GameOverText.setPosition(70, 200);
	GameOverText.setFillColor(Color(80, 200, 30, 255));
	GameOverText.setOutlineColor(Color(0, 0, 0, 255));
	GameOverText.setOutlineThickness(2);

	// ��������� �������� ������
	Text AltitudeText;
	AltitudeText.setFont(FontDimboRegular);
	AltitudeText.setFillColor(Color(80, 200, 30, 255));
	AltitudeText.setOutlineColor(Color(0, 0, 0, 255));
	AltitudeText.setOutlineThickness(2);
	
	Sprite sBackground(TextureBackground), sPlat(TextureRegularPlat), sPers(TextureDoodleR);

	point plat[20];

	for (int i = 0; i < 10; i++)
	{
		plat[i].x = rand() % 400;
		plat[i].y = rand() % 533;
	}

	int x = 100, y = 100, h = 200;
	float dx = 0, dy = 0;
	float AltitudeCount = 0;
	bool PauseFlag = false; // ���� ���������� ����� �� ���� �� �����
	bool GameOverFlag = false;

	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();

			// ���� �������� ������ P, ���� ���������� ���� �� ����� �������� �� ���������������
			if ((e.type == Event::KeyReleased) && (e.key.code == Keyboard::P))
				PauseFlag = !PauseFlag;

			if ((e.type == Event::KeyReleased) && (e.key.code == Keyboard::Space) && (GameOverFlag == true))
			{
				// ������ ������ ������������� (������ ���������������� ������)
				y = 0;
				dy = 0;
				GameOverFlag = false;
			}
		}
		
		// ���� ���� �� ����� �� �����, ��������� �������� ����
		if (!PauseFlag && !GameOverFlag)
		{
			// ���������� ����������
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				x += 3;
				if (x >= 400) x = 0;
				sPers.setTexture(TextureDoodleR);
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				x -= 3;
				if (x < 0) x = 400;
				sPers.setTexture(TextureDoodleL);
			}

			// ���������� ������
			dy += 0.2;
			y += dy;
			
			// ����� ���� ��� �������
			if (y > 500) GameOverFlag = true;

			// ���� �������� �������� ���������� ������ (���� ������� [533 - h]), �������� ��������� ����
			if (y < h)
			{
				for (int i = 0; i < 10; i++)
				{
					y = h;
					plat[i].y = plat[i].y - dy;

					//���� ��������� ���������� ���� ������� ������, ������� ����� ��������� ������
					if (plat[i].y > 533)
					{
						plat[i].y = 0;
						plat[i].x = rand() % 400;
					}
				}

				AltitudeCount -= dy; //�������� �������� �������, �.�. ������� ��������� ���������� ����
			}
				

			// ������ �� ���������
			for (int i = 0; i < 10; i++)
			{
				if ((x + 50 > plat[i].x) && (x + 20 < plat[i].x + 68) && (y + 70 > plat[i].y) && (y + 70 < plat[i].y + 14) && (dy > 0)) dy = -10;
			}
		}

		// ������ ��������� �����
		sPers.setPosition(x, y);
		AltitudeText.setPosition(350, 10);
		AltitudeText.setString(std::to_string((int)round(AltitudeCount/10)));

		app.draw(sBackground);
		
		for (int i = 0; i < 10; i++)
		{
			sPlat.setPosition(plat[i].x, plat[i].y);
			app.draw(sPlat);
		}

		app.draw(sPers);
		app.draw(AltitudeText);

		if (GameOverFlag) app.draw(GameOverText);

		app.display();
		// ����� ��������� �����
	}

	return 0;
}