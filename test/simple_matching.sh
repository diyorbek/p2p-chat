#!/bin/bash
trap "exit" INT TERM ERR
trap "kill 0" EXIT

curr_dir="$( dirname -- "$0"; )"
rendezvous=$curr_dir/../build/rendezvous-server/rendezvous
client=$curr_dir/../build/client/client

delete_file(){
  if [ -f $1 ]
  then 
      rm $1
  fi 
}

delete_file 1.txt

$rendezvous & sleep 1

echo "Peer1
room-1" | $client -d -p 12345 | grep Peer >> 1.txt &
echo "Peer2
room-2" | $client -d -p 23456 | grep Peer >> 1.txt &
echo "Peer3
room-1" | $client -d -p 34567 | grep Peer >> 1.txt &
echo "Peer4
room-2" | $client -d -p 45678 | grep Peer >> 1.txt &

sleep 1

actual=$(cat 1.txt | grep -oE 'Peer\d .+' | sort)
expected="Peer1 127.0.0.1 12345
Peer2 127.0.0.1 23456
Peer3 127.0.0.1 34567
Peer4 127.0.0.1 45678"

rm 1.txt

# echo $expected
echo $actual

if [ "$expected" == "$actual" ]; then
  echo -e "\033[32msuccess"
else
  echo -e "\033[31mfailed"
  exit 1
fi
