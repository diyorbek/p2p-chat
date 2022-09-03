#!/bin/bash
trap "exit" INT TERM ERR
trap "kill 0" EXIT

curr_dir="$( dirname -- "$0"; )"
client=$curr_dir/../build/client/client

delete_file(){
  if [ -f $1 ]
  then 
      rm $1
  fi 
}

send(){
  sleep 0.5;
  echo $1
}

clear_snap(){
  cat $1 | tr "\r" "\n" | grep -E "Peer\d" | tr -d ' '
}

assert_equal(){
	if [ "$1" == "$2" ]; then
	  echo -e "\033[32msuccess"
	else
	  echo -e "\033[31mfailed"
	fi
}

delete_file 2.txt
delete_file 3.txt
delete_file in2
delete_file in3

mkfifo in2
mkfifo in3

tail -f in2 | $client -p 12345 -c Peer2 127.0.0.1 23456 >> 2.txt &
tail -f in3 | $client -p 23456 -c Peer1 127.0.0.1 12345 >> 3.txt &

sleep 0.5

send "Hey!" > in2
send "Hi!" > in3
send "Wassup?" >> in2
send "Nothing much" >> in3

act_snap2=`clear_snap 2.txt`
exp_snap2="Peer2:Hi!
Peer2:Nothingmuch"
act_snap3=`clear_snap 3.txt`
exp_snap3="Peer1:Hey!
Peer1:Wassup?"

echo $act_snap2
echo $exp_snap2
echo $act_snap3
echo $exp_snap3

assert_equal "$act_snap2" "$exp_snap2"
assert_equal "$act_snap3" "$exp_snap3"

rm in2
rm in3
rm 2.txt
rm 3.txt
