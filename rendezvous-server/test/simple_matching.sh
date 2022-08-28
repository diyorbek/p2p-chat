trap "exit" INT TERM ERR
trap "kill 0" EXIT

curr_dir="$( dirname -- "$0"; )"

delete_file(){
  if [ -f $1 ]
  then 
      rm $1
  fi 
}

delete_file 1.txt
delete_file 2.txt

$curr_dir/../../build/rendezvous-server/rendezvous  &
sleep 1

echo Peer1 room-1 | nc -p 12345 -u 0.0.0.0 8081 >> 1.txt &
echo Peer2 room-2 | nc -p 23456 -u 0.0.0.0 8081 >> 1.txt &
echo Peer3 room-1 | nc -p 34567 -u 0.0.0.0 8081 >> 1.txt &
echo Peer4 room-2 | nc -p 45678 -u 0.0.0.0 8081 >> 1.txt &

sleep 1

actual=`cat 1.txt | sort`
expected="Peer1 127.0.0.1 12345
Peer2 127.0.0.1 23456
Peer3 127.0.0.1 34567
Peer4 127.0.0.1 45678"

rm 1.txt

# echo $expected
# echo $actual

if [ "$expected" == "$actual" ]; then
  echo "\033[32msuccess"
else
  echo "\033[31mfailed"
fi
