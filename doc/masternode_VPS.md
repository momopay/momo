MOMO - How to Setup VPS Masternode?2018  simple Guide

IMPORTANT NOTES:
Many users are getting similar issue, so see 3 simple and MANDATORY NOTES before setting up:

momo.conf file on LOCAL wallet MUST BE EMPTY!
masternode.conf file on VPS wallet MUST BE EMPTY!
PRE_ENABLED status is NOT an issue, just restart local wallet and wait a few minutes.
--

This guide is for a single masternode, on a Ubuntu 14 64bit server (2GB RAM minimum) or 16.04 LTS. That will be controlled from the wallet on your local computer and all commands on VPS are running as root.

If you don’t have enough RAM, create 2–4GB of swap memory using these commands line by line:

cd /
sudo dd if=/dev/zero of=swapfile bs=1M count=3000
sudo mkswap swapfile
sudo swapon swapfile
sudo nano etc/fstab
/swapfile none swap sw 0 0

Basic requirements
1001 MOC (1 is for fee)
A main computer (containing the main wallet where your coins will be stored) Ubuntu, Windows or Mac
Masternode Server (The VPS that will be on 24/7) Access with root using putty or MacOS terminal
A unique VPS and IP address for EACH masternode
(For security reasons, you’re gonna need a different IP for each masternode you plan to host)
The basic reasoning for these requirements is that, you get to keep your MOC in your local wallet, and host your masternode remotely, securely.

For this guide, I’m going to refer to your main computer’s wallet as the main wallet, and the masternode wallet as the masternode wallets.

Setting up the main wallet
Go to your local computer where your main wallet is running with all your coins inside.

On menu, Go to FILE -> RECEIVING ADDRESSES -> click NEW and put your Label (alias), we are using "MN1" on this tutorial. Feel free to choice or use the same.

Then copy the address and send EXACTLY 1000 MOC to this address, you will be just send a payment to yourself.
Wait for 15 confirmations, then go to the debug console and use this command:

masternode outputs

You should see one line corresponding to the transaction id (tx_id) of your 1000 coins with a digit identifier (digit). Save these two strings in a text file.

Note that if you get more than 1 line, it’s because you made multiple 1000 coins transactions, with the tx_id and digit associated.

Now we have to create the masternode private key to link the main wallet and the VPS masternode. Type in the debug console :

masternode genkey

Copy this key somewhere. It will be referred as masternodeprivkey.

Next, you have to go to the data directory of your main wallet (in Linux it’s located at /home/user/.momocore) or you can open it by wallet GUI by Tools - Open masternode configuration file. So type :

port base on real port
MN1 IP:7717 masternodeprivkey tx_id digit

Put your data correctly, save it and close. Restart your main wallet.

Note that each line of the masternode.conf file corresponds to one masternode.

Setting up the Masternode wallet
First, you’ll have to install the required packages with this 1-line Setup as root:

apt-get update;apt-get upgrade -y;apt-get dist-upgrade -y;apt-get install nano htop git -y;apt-get install build-essential libtool autotools-dev automake pkg-config -y;apt-get install libssl-dev libevent-dev bsdmainutils software-properties-common -y;apt-get install libboost-all-dev -y;apt-get install libzmq3-dev libminiupnpc-dev libssl-dev libevent-dev -y;add-apt-repository ppa:bitcoin/bitcoin -y;apt-get update;apt-get install libdb4.8-dev libdb4.8++-dev -y;

Now we have to build the wallet. Clone the Github momo repository from here.

sudo apt-get install git
git clone https://github.com/momopay/momo.git

Then navigate to the newly created momo folder and execute the following, line by line:

cd momo
chmod 755 autogen.sh
./autogen.sh
./configure
chmod 755 share/genbuild.sh
make

This process can take a while, it will compile the momo wallet, you can got errors if your VPS has less than 2GB RAM, so you need to setup swap and compile all over again.

After complete, you need to start daemon only to create data folder and files, wait a few seconds and stop the daemon so you can edit the conf file on next step, use the follow comandos to navigate to src folder to do it:

cd src/
./momod -daemon

Wait a few seconds then stops with:

./momo-cli stop

Navigate to the data directory by typing

cd /root/.momocore
nano momo.conf

Now copy paste the following configuration :

rpcuser=user
rpcpassword=pass
rpcallowip=127.0.0.1
listen=1
server=1
daemon=1
rpcport=6615
staking=0
externalip=IP:7717
masternode=1
masternodeprivkey=masternodeprivkey

You need to change IP to your VPS IP address, the masternodeprivkey is the one that you got from the main wallet. Choose whatever you like for user and password. Note that the port should be 7717 for all momo masternodes and rpcport is 6615 for sentinel.

Type Ctrl + X => Y => Enter. The file momo.conf is now saved.

If you have a firewall running, you need to open the 7717 and 6615 port :

sudo ufw allow 7717/tcp
sudo ufw allow 6615/tcp

Now start again your masternode wallet by navigating through your momo folder.

cd /root/momo/src/
./momod -daemon

Wait like 10 mins for your wallet to download the blockchain. You can check the progress with the following command :

./momo-cli getblockcount

The block number has to catch up with the latest on the explorer.

Starting the Masternode
Go back to your main wallet, to the Masternode tab.

You need to wait for 15 confirmations in order to start the masternode. Select the line corresponding to the masternode.

If the Masternode tab isn’t showing, you need to go to Options>Wallet>Show Masternodes Tab.

Click “start-alias”. Your masternode should be now up and running !

Checking the Masternode status
You can check the masternode status by going to the masternode wallet and typing:

./momo-cli masternode status

If your masternode is running it should print “Masternode successfully started”.

Now you need SENTINEL to fix WATCHDOG EXPIRED issue, run this 1 line for it:

cd /root/.momocore;wget https://github.com/momopay/sentinel/releases/download/v1.1.0.3/sentinel-lin64 ;chmod +x sentinel-lin64;

Before run, install and run screen:

apt-get install screen
screen
./sentinel-lin64

You will see it running, so then you can close window. Screen will make it running from behind, so wait around 30min to see ENABLED instead of WATCHDOG EXPIRED.

You can also check your MN status by local wallet - tools - debug console, just type:

masternode list full XXXXX

(Where XXXXX is yours first 5 character of TX_ID).

I hope this guide was usefull to you. You can ask me some more questions on the official momo discord channel. My username is juniormasters. Please if you found some error give me feedback so i can fix it!

If you liked my work and want to thank me i’ll be happy for a tip :)

momo address donations : MS24tp5vxNLP6mNGrra4r9JM16h6SnDfTk

any question,Please contact: alemic@momocash.org
