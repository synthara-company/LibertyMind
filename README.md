# Synthara

A pure terminal-based C++ application for interacting with Google's Gemini language models.

## Features

- Support for Google's Gemini models
- Secure API key and token storage
- Simple terminal-based UI using ncurses
- Chat history management
- Markdown export functionality
- Customizable system messages
- Theme customization options

## Supported Models

### Google
#### Gemini 2.0 Models
- `gemini-2.0-flash-lite` - Cost-effective model for simple tasks

#### Gemini 1.5 Models
- `gemini-1.5-pro` - Google's versatile model with long context
- `gemini-1.5-flash` - Faster, more efficient Google model

## Dependencies

- C++17 compatible compiler
- CMake (3.10 or higher)
- libcurl
- ncurses
- nlohmann/json (automatically fetched by CMake)

## Building

```bash
# Create a build directory
mkdir build
cd build

# Configure and build
cmake ..
make

# Install (optional)
sudo make install
```

## Usage

```bash
# Run the application
./synthara
```

### Navigation

- Use arrow keys to navigate menus
- Press Enter to select an option
- Press Escape to go back to the previous screen
- Press F10 to exit the application

### Setup

1. Select Google as your provider
2. Select a model from the available Gemini models
3. Enter your Google API key
4. Configure your API token
5. Set port (default: 8080)
6. (Optional) Set a custom system message
7. Start chatting!

## API Keys

You'll need to obtain an API key from Google:

- Google: https://ai.google.dev/

## Markdown Export

Synthara allows you to export your chat conversations to Markdown files:
- Press the `M` key during a chat session to export
- Choose a file path or use the default location
- The exported file includes timestamp, system instructions, and the complete conversation

## Theme Customization

Synthara offers three theme options:
- Light Theme - Clean, bright interface
- Dark Theme - Reduced eye strain in low light
- System Theme - Follows your system preferences

You can also customize individual interface elements:
- Header colors
- Selected item colors
- Message colors
- Error colors

## About

Synthara is developed by LibertyMind, a research service provider and computer software company specializing in artificial intelligence and natural language processing.

## License

apache-2.0
