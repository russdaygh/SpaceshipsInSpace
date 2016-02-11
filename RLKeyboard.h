#pragma once

namespace RustyLib
{
	namespace Input
	{
		//Enumerator for ASCII key values
		//Values must never exceed Keyboard::NUM_KEYS
		enum RLKeyCode{
			Return = 13,
			Escape = 27,
			SpaceBar = 32
		};
		typedef RLKeyCode KeyCode;

		class RLKeyboard
		{
		private:
			//Constants
			static const unsigned int NUM_KEYS = 128;

			//Hidden ctors and dtors
			RLKeyboard(void );
			~RLKeyboard(void );

			//Key array
			static bool keys[NUM_KEYS];

		public:
			//Key Response Methods
			static void HandleKeyDown(const char key);
			static void HandleKeyUp(const char key);

			//Key Value Retrieval Method
			static const bool IsKeyDown(const KeyCode key);
		};

		typedef RLKeyboard Keyboard;
	}
}
