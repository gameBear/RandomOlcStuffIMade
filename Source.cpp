#include "olcPixelGameEngine.h"
static int texSize = 12;
static int screenH = 400, screenW = 600;
class Example : public olc::PixelGameEngine
{
public:


	bool Manu1Out = false, Manu2Out = false;

	int Manu1Pos = 0, Manu2Pos = 0;

	int Manu1Selected = 0, Manu2Selected = 0;

	std::vector<std::string> Menu1List;
	std::vector<std::string> Menu2List;
	
	Example()
	{
		sAppName = "Easy MenuEditor";
	}

private:
	//Add some items to the first menu vector
	bool loadMenu1List() {
		Menu1List.push_back("Run");
		Menu1List.push_back("Stay");
		Menu1List.push_back("Sit");
		Menu1List.push_back("Good boy!");
		Menu1List.push_back("EXIT");
		return true;
	}

	//Add some items to the second menu vector
	bool LoadMenu2List() {
		Menu2List.push_back("Option 1");
		Menu2List.push_back("Option 2");
		Menu2List.push_back("Option 3");
		Menu2List.push_back("Option 4");
		Menu2List.push_back("Option 5");
		Menu2List.push_back("Option 6");
		Menu2List.push_back("Option 7");
		Menu2List.push_back("Option 8");
		Menu2List.push_back("Option 9");
		Menu2List.push_back("Option 10");
		Menu2List.push_back("Option 11");
		return true;
	}

	void DrawBox(int x, int y, int w, int h, olc::Pixel boxColor, olc::Pixel outline) {

		DrawRect(x, y, w, h, outline); 
		DrawRect(x + 1, y + 1, w - 2, h - 2, outline);
		FillRect(x + 2, y + 2, w - 4, h - 4, boxColor);
	}

	//ind i the horizontal position of the menu, menuName gives, menuItems is a vector of strings with items
	int DrawMenu(int ind, std::string menuName, std::vector<std::string> &menuItems, bool &isOut, int MouseX, int MouseY, bool clicked, int &pos) {
		int itemClicked = -1;
		int Place = 1;
		int Scroolable = 0;
		if (isOut) Place = menuItems.size()+1;
		if (Place > 7) {
			Scroolable = Place - 7;
			Place = 7;
		}

		DrawBox(2 + (ind * 140), 2, 126, 6 + Place * texSize, olc::GREY, olc::DARK_GREY);

		if (MouseX < 100 + (ind * 140) && MouseX > 4 + (ind * 140) && MouseY < 2 + Place * texSize && MouseY > 4) {
			int yPos = 2 + Place * texSize, stuf = Place - 1;
			while (yPos > MouseY) {
				yPos -= texSize;
				stuf--;
			}
			FillRect(4 + (ind * 140), yPos, 100, 12, olc::BLUE);
			if (clicked && MouseX < 104 + (ind * 140) && MouseY > 4 + texSize) {
				itemClicked = stuf + pos;
			}
		}


		if (Scroolable > 0) {
			DrawLine(2 + (ind * 140) + 115, 22, 2 + (ind * 140) + 115, 1 + (Place - 1) * texSize, olc::BLACK);
			if (pos != 0) {
				FillRect(2 + (ind * 140) + 110, 10, 10, 10, olc::DARK_GREEN);
				if (clicked && MouseX > 2 + (ind * 140) + 110 && MouseX < 2 + (ind * 140) + 110 + 10 && MouseY > 10 && MouseY < 20) {
					pos--;
					FillRect(2 + (ind * 140) + 110, 10, 10, 10, olc::RED);
				}
			}
			if (pos != Scroolable) {
				FillRect(2 + (ind * 140) + 110, 4 + (Place-1) * texSize, 10, 10, olc::DARK_GREEN);
				if (clicked && MouseX > (2 + (ind * 140) + 110) && MouseX < (2 + (ind * 140) + 120) + 120 && MouseY > (4 + (Place - 1) * texSize) && MouseY < (14 + (Place - 1) * texSize)) {
					pos++;
					FillRect(2 + (ind * 140) + 110, 4 + (Place - 1) * texSize, 10, 10, olc::RED);
				}
			}

		}

		int index = 0;
		Place = 0;

		
		if (isOut){
			DrawString(5 + (ind * 140), 5 + Place * texSize, menuName + " v", olc::BLACK);
			if (clicked && MouseX < 100 + (ind * 140) && MouseX > 4 + (ind * 140) && MouseY < 2 + (1 + Place) * texSize && MouseY> 2 + Place * texSize) isOut = false;
			Place++;
			for (std::vector<std::string>::iterator it = menuItems.begin() + pos; it != menuItems.end(); it++) {
				DrawString(5 + (ind * 140), 5 + Place * texSize, menuItems.at(index+pos), olc::BLUE);
				index++;
				Place++;
				if (index >= 6) it = menuItems.end() - 1;
			}
		}
		else {
			DrawString(5 + (ind * 140), 5 + Place * texSize, menuName + " >", olc::BLACK);
			if (clicked && MouseX < 122 + (ind * 140) && MouseX > 4 + (ind * 140) && MouseY < 2 + (1 + Place) * texSize && MouseY > 2 + Place * texSize) isOut = true;
		}

		return itemClicked;
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		//First, load our menu lists
		if (!loadMenu1List()) return false;
		if (!LoadMenu2List()) return false;


		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		//We store the mouse input we need, no need to get it each time.
		int x = GetMouseX();
		int y = GetMouseY();
		bool clicked = GetMouse(0).bPressed;

		//clear the screen
		Clear(olc::DARK_BLUE);


		int WasMenu1Clicked = DrawMenu(0, "FILES", Menu1List, Manu1Out, x, y, clicked, Manu1Pos);
		if (WasMenu1Clicked != -1) Manu1Selected = WasMenu1Clicked;

		int WasMenu2Clicked = DrawMenu(1, "OPTIONS", Menu2List, Manu2Out, x, y, clicked, Manu2Pos);
		if (WasMenu2Clicked != -1) Manu2Selected = WasMenu2Clicked;

		std::string cMenu1 = "The last file click was: \"" + Menu1List.at(Manu1Selected) + "\"";
		std::string cMenu2 = "The option click was: \"" + Menu2List.at(Manu2Selected) + "\"";

		DrawBox(2 + 140 * 2, 2, 300, 50, olc::GREY, olc::DARK_GREY);
		DrawString(5 + 140 * 2, 5, cMenu1, olc::BLACK);
		DrawString(5 + 140 * 2, 17, cMenu2, olc::BLACK);

		if(Menu1List.at(Manu1Selected) != "EXIT") return true;

		else return false;
		
	}
};


int main()
{
	Example demo;
	if (demo.Construct(screenW, screenH, 1, 1))
		demo.Start();

	return 0;
}
