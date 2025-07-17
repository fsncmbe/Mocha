#define MOCHA_INPUT

#include <mocha.hpp>
#include <utils.hpp>
#include <core.hpp>

namespace mocha 
{
bool checkKeyOutOfBounds(int key)
{
  if (key > MAX_KEYS)
  {
    log(LogLevel::ERROR, "Key out of bounds!");
    return false;
  }
  return true;
}

bool getKeyPressed(int key)
{
  return  core.input.current_key_states[key] 
  &&      !core.input.previous_key_states[key] 
  &&      checkKeyOutOfBounds(key);
}

bool getKeyDown(int key)
{
  return  core.input.current_key_states[key]
  &&      checkKeyOutOfBounds(key);
}

bool getKeyReleased(int key)
{
  return  !core.input.current_key_states[key]
  &&      core.input.previous_key_states[key]
  &&      checkKeyOutOfBounds(key);
}

bool getKeyUp(int key)
{
  return  !core.input.current_key_states[key]
  &&      checkKeyOutOfBounds(key);
}

KeyState getKeyState(int key)
{
  if (getKeyPressed(key)) return KeyState::kPressed;
  if (getKeyDown(key)) return KeyState::kDown;
  if (getKeyReleased(key)) return KeyState::kReleased;
  return KeyState::kUp;
}

}