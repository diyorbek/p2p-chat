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
room-1" | $client -d | grep Peer >> 1.txt &
echo "Peer2
room-2" | $client -d | grep Peer >> 1.txt &
echo "Peer3
room-1" | $client -d | grep Peer >> 1.txt &
echo "Peer4
room-2" | $client -d | grep Peer >> 1.txt &

sleep 1

actual=$(cat 1.txt | grep -oe 'Peer\d' | sort)
expected="Peer1
Peer2
Peer3
Peer4"

rm 1.txt

# echo $expected
# echo $actual

if [ "$expected" == "$actual" ]; then
  echo -e "\033[32msuccess"
else
  echo -e "\033[31mfailed"
fi
