// Pomodoro timer
// 25 minutes work, 5 minute break, x 4.

#include <iostream>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
using namespace chrono;

// terminal manipulation functions
void enableRawMode(termios& orig) {
  termios raw;
  tcgetattr(STDIN_FILENO, &orig);
  raw = orig;
  raw.c_lflag &= !(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode(const termios& orig) {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
}

void setNonBlocking(bool enable) {
  int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
  if (enable) {
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
  } else {
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
  }
}

// check if a key was pressed
char getKeyPress() {
  char c;
  if (read(STDIN_FILENO, &c, 1) == 1) {
    return c;
  }
  return '\0';
}

// display the countdown (overwrites current line)
void displayTime(int minutes, int seconds, const string& phase) {
  cout << "\033[2K\r" << phase << " - "
       << minutes << "m " << seconds << "s remaining | [p]ause [q]uit"
       << flush;
}

// run the countdown timer
bool runTimer(int totalSeconds, const string& phase) {
  termios orig;
  enableRawMode(orig);
  setNonBlocking(true);

  auto startTime = steady_clock::now();
  int elapsed = 0;
  bool paused = false;
  auto pauseStart = steady_clock::now();
  int pausedTime = 0;

  while (elapsed < totalSeconds) {
    // handle keypresses 
    char key = getKeyPress();
    if (key == 'q' || key == 'Q') {
      disableRawMode(orig);
      setNonBlocking(false);
      return false;
    }
    if (key == 'p' || key == 'P') {
      paused = !paused;
      if (paused) {
        pauseStart = steady_clock::now();
        cout << "\033[2K\r" << "⏸ PAUSED - Press 'p' to resume" << flush;
      } else {
        pausedTime += duration_cast<seconds>(steady_clock::now() - pauseStart).count();
      }
    }

    if (!paused) {
      // calculate time remaining
      auto now = steady_clock::now();
      elapsed = duration_cast<seconds>(now - startTime).count() - pausedTime;
      int remaining = totalSeconds - elapsed;
      int mins = remaining / 60;
      int secs = remaining % 60;

      displayTime(mins, secs, phase);
    }

    this_thread::sleep_for(milliseconds(100));
  }

  // timer complete - ring bell
  cout << "\a" << endl;

  disableRawMode(orig);
  setNonBlocking(false);
  return true;
}

int main() {
  const int WORK_TIME = 25 * 60;
  const int SHORT_BREAK = 5 * 60;
  const int LONG_BREAK = 60 * 60;
  const int ROUNDS_BEFORE_LONG_BREAK = 4;

  int completedRounds = 0;

  cout << "🍅 Pomodoro Timer\n";
  cout << "Controls: [p]ause/resume  [q]uit\n\n";

  while (true) {
    // work session
    cout << "\n>>> Round " << (completedRounds + 1) << " - Starting work session...\n";
    if (!runTimer(WORK_TIME, "⚡WORK")) {
      cout << "\nQuitting...\n";
      break;
    }

    completedRounds++;
    cout << "\n✓ Work session complete!\n";

    // check if long break is needed
    if (completedRounds % ROUNDS_BEFORE_LONG_BREAK == 0) {
      cout << "\n🛑 You've completed " << ROUNDS_BEFORE_LONG_BREAK
           << " rounds! Time for a mandatory 1-hour break.\n";
      cout << "Starting long break...\n";

      if (!runTimer(LONG_BREAK, "🌴 LONG BREAK")) {
        cout << "\nQuitting...\n";
        break;
      }

      cout << "\n✓ Long break complete! Ready for more rounds.\n";
    } else {
      // short break
      cout << "\nStarting short break...\n";
      if (!runTimer(SHORT_BREAK, "☕ BREAK")) {
        cout << "\nQuitting...\n";
        break;
      }
      cout << "\n✓ Break complete!\n";
    }
  }

  return 0;
}
