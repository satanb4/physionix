![physionix-image](media/Physionix-logo/cover.png)

![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/satanb4/physionix/c-cpp.yml?style=for-the-badge)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/satanb4/physionix?style=for-the-badge)
![GitHub repo size](https://img.shields.io/github/repo-size/satanb4/physionix?style=for-the-badge)

![GitHub language count](https://img.shields.io/github/languages/count/satanb4/physionix?style=for-the-badge)
![GitHub top language](https://img.shields.io/github/languages/top/satanb4/physionix?style=for-the-badge)
![GitHub issues](https://img.shields.io/github/issues/satanb4/physionix?style=for-the-badge)  

![GitHub forks](https://img.shields.io/github/forks/satanb4/physionix?style=for-the-badge)
![GitHub Repo stars](https://img.shields.io/github/stars/satanb4/physionix?style=for-the-badge)
![GitHub commit activity](https://img.shields.io/github/commit-activity/w/satanb4/physionix?style=for-the-badge)
![GitHub](https://img.shields.io/github/license/satanb4/physionix?style=for-the-badge)


## Your personal physiotherapist
**Consult your physiotherapist from the comfort of your home**

Our application is capable of detecting even the slightest motion of your muscles, helping you to complete your physio regimen to help you make the most of it. No more worries about over-straining or under-engaging your muscles. Our application is focused on delivering optimum power to aid you in your exercises and to help you become a better you.

We have releases with our binaries for the Raspberry Pi 4B. You can download them from the [Releases](https://github.com/satanb4/physionix/releases) section.

# How to Build
Building this repository is easy. All code is built with a simple build script that takes care of dependencies as well.  
Make sure to have root access to your system.  
Just follow the steps below:
- Login to your Raspberry Pi Machine  
**If Required** Install Git
   ```sh
   sudo apt-get update
   sudo apt-get install -y git
   ```
- Clone the repository
   ```sh
   $ git clone https://github.com/satanb4/physionix.git
   $ cd $HOME/physionix
   ```
- Install the dependencies and build
  ```sh
  chmod +x build.sh
  ./build.sh
  ```
- The application will be present in the `build` folder

## Features
- :white_check_mark: Build Scripts for Raspberry Pi and Linux
- :white_check_mark: Web UI for User Interaction
- :white_check_mark: EMG Acquisition using ADS1115 ADC
- :white_check_mark: Interrupt based data acquisition
- :white_check_mark: Thread based data handling
- :white_check_mark: EMG Signal Processing with FFT
- :white_check_mark: Signal Classification
- :white_check_mark: Handoff Back to Main Thread
- :white_check_mark: Update Web UI with Signal Classification
- :white_check_mark: Thread based management for servo control and Actuation

## Contributions to Modules

| Module                                                                | Contributor |
| -------------------------------------------------                     | ---- |
| :white_check_mark: ADS1115 / Build            | [Midhun Mathew](https://github.com/midhunjac)         |
| :white_check_mark: EMGData Module             | [Midhun Mathew](https://github.com/midhunjac)         |
| :white_check_mark: Core Integration           | [Midhun Mathew](https://github.com/midhunjac)         |
| :white_check_mark: Top-Level Threading        | [Midhun Mathew](https://github.com/midhunjac)         |
| :white_check_mark: EMGSignalProcessing Module | [Sayan Bandyopadhyay](https://github.com/satanb4)     |
| :white_check_mark: WebAPI Module              | [Sayan Bandyopadhyay](https://github.com/satanb4)     |
| :white_check_mark: Build Scripts (Sh)/ README | [Sayan Bandyopadhyay](https://github.com/satanb4)     |
| :white_check_mark: Github Actions             | [Sayan Bandyopadhyay](https://github.com/satanb4)     |
| :white_check_mark: EMG Actuation Module       | [Sri Krishna Sumanth](https://github.com/Sumanth0201) |
| :white_check_mark: High-level Diagrams        | [Sri Krishna Sumanth](https://github.com/Sumanth0201) |
| :white_check_mark: Media and Marketings       | [Sri Krishna Sumanth](https://github.com/Sumanth0201) |

### Schematics
![Schematics](https://user-images.githubusercontent.com/123675167/229873061-d8c7acad-f5ce-4a66-b9ea-08cc2b337cba.png)

  
# Our Socials
![Twitter URL](https://img.shields.io/twitter/url?style=social&url=https%3A%2F%2Ftwitter.com%2FThe_Physionix)
![YouTube Channel Views](https://img.shields.io/youtube/channel/views/@The_Physionix?style=social)  
[Instagram](https://www.instagram.com/the_physionix/)  
[Youtube Channel](https://www.youtube.com/@The_Physionix/featured)  
[Twitter](https://twitter.com/The_Physionix)  

# Contributing
We are open to `enhancements` & `bug-fixes` 😊    
![GitHub Contributors Image](https://contrib.rocks/image?repo=satanb4/physionix)

# Contact Us 📧
- This project is developed and maintained by :
  * [Sayan Bandyopadhyay](https://github.com/satanb4)
  * [Midhun Mathew](https://github.com/midhunjac) 
  * [Sri Krishna Sumanth](https://github.com/Sumanth0201)
 
 ---
## License

MIT License

Copyright (c) [2023] [Sayan Bandyopadhyay]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

```
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
