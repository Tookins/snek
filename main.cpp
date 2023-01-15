// snake game written in c++ using sdl2 and aseprite
// TODO:
// get user text input from the game over screen
// create high score page
// Add difficulty scaling
// fix keyboard input

#include <SDL2/SDL.h>		//enables sdl windows and rendering
#include <SDL2/SDL_image.h> //enables the loading of png files
#include <SDL2/SDL_mixer.h> //enables the use of audio
#include <SDL2/SDL_ttf.h>	//enables the use of text output
#include <stdio.h>			//used for error messages
#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <cstdlib>
#include <algorithm>
#include "GameRenderer.hpp"
#include "GameObject.hpp"
#include "Snake.hpp"

// game constants
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define TEXTURE_WIDTH 32
#define TEXTURE_HEIGHT 32

// void function pointer for music
int musicPlaying = 1;
void musicFinished()
{
	musicPlaying = 0;
}
// sort function for score data
bool sortByScore(const std::pair<std::string, std::string> &a, const std::pair<std::string, std::string> &b)
{
	return (std::stoi(a.second) < std::stoi(b.second));
}
// game entry point
int main(int argc, char *argv[])
{

	// initialize SDL, SDL_image, SDL_ttf, and SDL_mixer
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("SDL failed to init! SDL Error: %s\n", SDL_GetError());
	}
	if (!(IMG_Init(IMG_INIT_PNG)))
	{
		printf("IMG failed to init! IMG Error: %s\n", IMG_GetError());
	}
	if (TTF_Init() == -1)
	{
		printf("TTF failed to init! TTF Error: %s\n", TTF_GetError());
	}

	// code copied from mixer_tutorial
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
	{
		fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
		exit(1);
	}

	Mix_Music *sound = nullptr;
	sound = Mix_LoadMUS("audio/Snake_Charmer.ogg");
	if (sound == nullptr)
	{
		fprintf(stderr, "unable to load ogg file: %s\n", Mix_GetError());
		return 1;
	}

	int channel;
	channel = Mix_PlayMusic(sound, -1);
	if (channel == -1)
	{
		fprintf(stderr, "Unable to play ogg file: %s\n", Mix_GetError());
		return 1;
	}
	// void function pointer for music
	Mix_HookMusicFinished(musicFinished);

	// end of code copied from mixer_tutorial

	// load in font for game over screen
	TTF_Font *highScoreFont = TTF_OpenFont("./fonts/joystix_monospace.ttf", 32);
	if (highScoreFont == nullptr)
	{
		printf("failed to load font");
	}

	// initialize render window
	GameRenderer gWindow("GAME v0.1", SCREEN_WIDTH, SCREEN_HEIGHT);

	// load in game textures
	SDL_Texture *headTexture = gWindow.loadTexture("images/snekkyBoy.png");
	SDL_Texture *bodyTexture = gWindow.loadTexture("images/snekBody.png");
	SDL_Texture *foodTexture = gWindow.loadTexture("images/snekFood.png");
	SDL_Texture *highScoreTex = gWindow.loadTexture("images/game_over.png");
	SDL_Texture *gameBackground = gWindow.loadTexture("images/game_background.png");
	SDL_Texture *gameTitle = gWindow.loadTexture("images/title_screen.png");

	// game state flags
	bool appRunning = true;
	bool title = false;
	bool gameRunning = false;
	bool gameOver = false;
	bool highScore = false;



	// intitialize the game instance
	while (appRunning)
	{
		// start with title screen
		title = true;
		while (title)
		{
			// display the title screen
			gWindow.clear();
			gWindow.renderImage(gameTitle, nullptr, nullptr);
			gWindow.display();

			// check for mouse event to start the game
			SDL_Event initEvent;
			while (SDL_PollEvent(&initEvent) != 0)
			{
				// closes the window if user clicks the 'x' button
				if (initEvent.type == SDL_QUIT)
				{
					title = false;
					appRunning = false;
					break;
				}
				// set the game start flag to true to enter the game loop
				else if (initEvent.type == SDL_KEYDOWN)
				{
					gameRunning = true;
					title = false;
					break;
				}
			}
		}

		// close the game if user quits from the
		// title screen
		if (!appRunning)
		{
			continue;
		}

		// initialize the game instance
		GameObject *instance = new GameObject();
		Snake *snek = new Snake(1, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, TEXTURE_HEIGHT);
		gWindow.clear();

		// set game speed variable
		int fps = 6;

		// initialize rectangles that will
		// be used to render the snake and
		// render the head of the snake in its initial position
		SDL_Rect headRect = snek->getHead();
		SDL_Rect bodyRect;
		gWindow.renderImage(headTexture, nullptr, &headRect);

		// render the snake food in its initial position
		SDL_Rect target = {TEXTURE_WIDTH * (rand() % (SCREEN_WIDTH / TEXTURE_WIDTH - 2)) + TEXTURE_WIDTH,
						   TEXTURE_HEIGHT * (rand() % (SCREEN_HEIGHT / TEXTURE_HEIGHT - 2)) + TEXTURE_HEIGHT,
						   TEXTURE_WIDTH, TEXTURE_HEIGHT};
		gWindow.renderImage(gWindow.loadTexture("images/snekFood.png"), nullptr, &target);

		// draw the initial configuration of the game
		gWindow.display();

		// used to get the time each loop begins for controlling
		// framerate
		Uint32 startingTick;

		// main loop
		while (gameRunning)
		{
			// event for game inputs
			SDL_Event gameEvent;

			// get time from initial tick
			startingTick = SDL_GetTicks();

			// flag for checking if snake hits the target (food)
			bool onTarget = false;

			// reads through the event queue and moves according to arrow keys
			// or closes on esc key
			while (SDL_PollEvent(&gameEvent) != 0)
			{
				// closes the window if user clicks the 'x' button
				if (gameEvent.type == SDL_QUIT)
				{
					gameRunning = false;
					appRunning = false;
				}
				// when a key is pressed, changes direction or closes the program
				else if (gameEvent.type == SDL_KEYDOWN)
				{
					switch (gameEvent.key.keysym.sym)
					{
					case SDLK_UP:
						if (snek->getVY() == 0)
						{
							snek->setVel(0, -1 * TEXTURE_HEIGHT);
						}
						break;

					case SDLK_DOWN:
						if (snek->getVY() == 0)
						{
							snek->setVel(0, TEXTURE_HEIGHT);
						}
						break;

					case SDLK_LEFT:
						if (snek->getVX() == 0)
						{
							snek->setVel(-1 * TEXTURE_WIDTH, 0);
						}
						break;

					case SDLK_RIGHT:
						if (snek->getVX() == 0)
						{
							snek->setVel(TEXTURE_WIDTH, 0);
						}
						break;

					case SDLK_ESCAPE:
						gameRunning = false;
						gameOver = true;
						continue;

					default:
						break;
					}
				}
			}

			// clear the window and render the textures for the next frame
			gWindow.clear();

			// if the snakes head is on the same rect as the target
			// set the update flag to true and change the position
			// of the target if necessary
			if (target.x == headRect.x && target.y == headRect.y)
			{
				onTarget = true;
				std::list<std::pair<int, int>> bodyCoords = snek->getBody();
				std::pair<int, int> targetCoords;
				targetCoords = {TEXTURE_WIDTH * (rand() % (SCREEN_WIDTH / TEXTURE_WIDTH - 2)) + TEXTURE_WIDTH,
								TEXTURE_HEIGHT * (rand() % (SCREEN_HEIGHT / TEXTURE_HEIGHT - 2)) + TEXTURE_HEIGHT};

				while (std::find(bodyCoords.begin(), bodyCoords.end(), targetCoords) != bodyCoords.end())
				{
					targetCoords = {TEXTURE_WIDTH * (rand() % (SCREEN_WIDTH / TEXTURE_WIDTH - 2)) + TEXTURE_WIDTH,
									TEXTURE_HEIGHT * (rand() % (SCREEN_HEIGHT / TEXTURE_HEIGHT - 2)) + TEXTURE_HEIGHT};
				}
				target.x = std::get<0>(targetCoords);
				target.y = std::get<1>(targetCoords);
				// if the score is a multiple of 5, increase the speed by 1 up to a max of 16
				if (snek->getLength() % 10 == 0)
				{
					fps++;
				}
			}

			// update the position of the snake and increase length by 1
			// if onTarget == true
			snek->update(onTarget);
			headRect = snek->getHead();

			// see if the snakes head collides with its body or the border
			// if true, exit game loop to game over loop
			// else, render the next frame
			std::list<std::pair<int, int>> snekCoordinates = snek->getBody();
			std::pair<int, int> headCoordinates = {headRect.x, headRect.y};
			if (headRect.x == 0 || headRect.x == SCREEN_WIDTH - TEXTURE_WIDTH || headRect.y == 0 || headRect.y == SCREEN_HEIGHT - TEXTURE_HEIGHT || std::find(snekCoordinates.begin(), snekCoordinates.end(), headCoordinates) != snekCoordinates.end())

			{
				gameRunning = false;
				gameOver = true;
			}
			else
			{
				// add the game background to the renderer
				gWindow.renderImage(gameBackground, nullptr, nullptr);
				// add the target texture to the renderer
				gWindow.renderImage(foodTexture, nullptr, &target);

				// add the head of the snake to the renderImageer
				gWindow.renderImage(headTexture, nullptr, &headRect);

				// add the body of the snake to the renderer
				for (std::pair<int, int> segment : snek->getBody())
				{
					bodyRect = {std::get<0>(segment), std::get<1>(segment), TEXTURE_WIDTH, TEXTURE_HEIGHT};
					gWindow.renderImage(bodyTexture, nullptr, &bodyRect);
				}
			}

			// display the renderer to the screen
			gWindow.display();

			// delay next frame to hold fps constant (defined at the top of main)
			if (1000 / fps > SDL_GetTicks() - startingTick)
			{
				SDL_Delay(1000 / fps - (SDL_GetTicks() - startingTick));
			}
		}
		// set the score based on the length of the snake
		instance->setScore(snek->getBody());

		// show a game over screen that displays the score and
		// prompts the user to enter a name for the scoreboard
		// then, update the scoreboard and move to that screen
		while (gameOver)
		{
			gWindow.clear();
			//render input hint
			std::string inputHint = "Press Enter to Continue";
			SDL_Rect hintRect = {64, 64, (int)inputHint.size() * 32, 32};
			gWindow.renderText(highScoreFont, inputHint, nullptr, &hintRect);
			// render the prompt to enter the username
			gWindow.renderImage(highScoreTex, nullptr, nullptr);
			std::string prompt = "Name: ";
			SDL_Rect promptRect = {SCREEN_WIDTH / 2 - 320, SCREEN_HEIGHT / 3 - 64, 192, 32};
			gWindow.renderText(highScoreFont, prompt, nullptr, &promptRect);
			// render the score prompt
			std::string scorePrompt = "Score: ";
			SDL_Rect scorePromptRect = {SCREEN_WIDTH / 2 - 320, SCREEN_HEIGHT * 2 / 3 - 64, 32 * 7, 32};
			gWindow.renderText(highScoreFont, scorePrompt, nullptr, &scorePromptRect);
			// render the score
			std::string score = std::to_string(instance->getScore());
			SDL_Rect scoreRect = {SCREEN_WIDTH / 2 - 320, SCREEN_HEIGHT * 2 / 3, 32, 32};
			gWindow.renderText(highScoreFont, score, nullptr, &scoreRect);
			gWindow.display();

			// receive text input for the username to add to the high score page
			SDL_Rect inputRect = {SCREEN_WIDTH / 2 - 320, SCREEN_HEIGHT / 3, 32, 32};
			SDL_SetTextInputRect(&inputRect);
			SDL_StartTextInput();
			bool inputDone = false;
			char *inputArray;
			std::string username = "";
			int size;
			while (!inputDone)
			{
				SDL_Event txtInputEvent;
				while (SDL_PollEvent(&txtInputEvent) != 0)
				{
					if (txtInputEvent.type == SDL_QUIT)
					{
						inputDone = true;
						gameOver = false;
						gameRunning = false;
					}
					else if (txtInputEvent.type == SDL_KEYDOWN && txtInputEvent.key.keysym.sym == SDLK_RETURN)
					{
						inputDone = true;
						gameOver = false;
						highScore = true;
					}
					else if (txtInputEvent.type == SDL_TEXTINPUT && username.size() < 10)
					{
						inputArray = txtInputEvent.text.text;
						username.append(inputArray);
						gWindow.clear();
						gWindow.renderImage(highScoreTex, nullptr, nullptr);
						gWindow.renderText(highScoreFont, inputHint, nullptr, &hintRect);
						gWindow.renderText(highScoreFont, prompt, nullptr, &promptRect);
						gWindow.renderText(highScoreFont, username, nullptr, &inputRect);
						gWindow.renderText(highScoreFont, scorePrompt, nullptr, &scorePromptRect);
						gWindow.renderText(highScoreFont, score, nullptr, &scoreRect);
						gWindow.display();
						// update inputrect
						inputRect.w += 32;
					}
				}
			}
			SDL_StopTextInput();
			// log game session info
			std::ofstream scoreLogFile;
			scoreLogFile.open("data/highscores.txt", std::ios::app);
			if (!scoreLogFile)
			{
				printf("log file could not be opened.");
				return 1;
			}
			scoreLogFile << username << " " << score << '\n';
			scoreLogFile.close();
		}

		// when the game instance is over, display the users name and score
		// to the game over screen
		// render the game over screen until the user quits or restarts the game
		while (highScore)
		{
			gWindow.clear();
			gWindow.renderImage(highScoreTex, nullptr, nullptr);
			//render input hint
			std::string inputHint = "Press Enter to Return to Menu";
			SDL_Rect hintRect = {64, 64, (int)inputHint.size() * 32, 32};
			gWindow.renderText(highScoreFont, inputHint, nullptr, &hintRect);
			// read the top ten scores from the log and put them on the screen
			std::ifstream scoreFile;
			scoreFile.open("data/highscores.txt");
			std::string buf;
			std::string uName;
			std::string uScore;
			std::vector<std::pair<std::string, std::string>> scores;
			while (!scoreFile.eof())
			{
				getline(scoreFile, buf);
				if (buf.size() != 0)
				{
					uName = buf.substr(0, buf.find(' '));
					uScore = buf.substr(buf.find(' '), buf.find('\n'));
					scores.push_back({uName, uScore});
				}
			}
			// sort the vector of string
			sort(scores.rbegin(), scores.rend(), sortByScore);
			std::string highScorePrompt = "Highscores:";
			SDL_Rect highScorePromptRect = {SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3 - 64, 352, 32};
			gWindow.renderText(highScoreFont, highScorePrompt, nullptr, &highScorePromptRect);
			SDL_Rect scoreRect;
			scoreRect = {SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, 320, 32};
			for (auto it : scores)
			{
				scoreRect.w = 32 * (it.first.size() + it.second.size() + 1);
				std::string result = it.first + " " + it.second;
				gWindow.renderText(highScoreFont, result, nullptr, &scoreRect);
				scoreRect.y += 64;
				if (scoreRect.y > SCREEN_HEIGHT - 128)
					break;
			}
			scoreFile.close();
			gWindow.display();
			// game over options (quit or play again)
			SDL_Event highScoreEvent;
			while (SDL_PollEvent(&highScoreEvent) != 0)
			{
				if (highScoreEvent.type == SDL_QUIT)
				{
					highScore = false;
					appRunning = false;
				}
				else if (highScoreEvent.type == SDL_KEYDOWN && highScoreEvent.key.keysym.sym == SDLK_RETURN)
				{
					highScore = false;
					appRunning = true;
				}
			}
		}

		// free up game entity memory
		delete snek;
		delete instance;
	}

	// destroy game window
	gWindow.destroy();
	// free music memory
	Mix_HaltMusic();
	Mix_FreeMusic(sound);
	Mix_CloseAudio();

	// destroy loaded textures
	SDL_DestroyTexture(headTexture);
	SDL_DestroyTexture(bodyTexture);
	SDL_DestroyTexture(foodTexture);
	SDL_DestroyTexture(highScoreTex);
	SDL_DestroyTexture(gameBackground);
	SDL_DestroyTexture(gameTitle);

	// quit sdl
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
