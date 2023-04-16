# For Setting up the Website

## Requirements
1. Install nginx  
   ```sh
    $ sudo apt-get update
    $ sudo apt-get install curl gnupg2 ca-certificates lsb-release ubuntu-keyring
    $ sudo apt-get install nginx
   ```
2. Edit the **nginx-sites-enabled-default** to point to your custom route
3. Run the setup script to copy setup the web-files `./setup.sh`
4. Run the nginx server 
   ```sh
    $ sudo service nginx start
   ```
5. Run the Physionix binary for the website to start taking data from the application
   ```sh
    $ build/Physionix
   ```