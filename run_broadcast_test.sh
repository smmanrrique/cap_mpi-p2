
PROGRAM="broadcast.out"
CSV_FILE="broadcast_test.csv"
HOSTFILE="host_lab004.txt"
PACKET_SIZES=(50)
BROADCAST_VALUES=(0 1)
NUMBER_PROCCESS=(8)


for p in "${NUMBER_PROCCESS[@]}"
    do
        for j in "${BROADCAST_VALUES[@]}"
            do
                echo "PACKET_SIZE,TYPE,RANK_SENDER,TIME,RUNNING_TIME" > "results/"$p"proc_"$j"bcast_"$CSV_FILE"";
                for i in "${PACKET_SIZES[@]}" 
                    do
                        # echo "mpirun -n "$p" --hostfile "$HOSTFILE" -mca btl_tcp_if_include br0 "$PROGRAM" "$i" "$j"";
                        mpirun -n "$p" --hostfile "$HOSTFILE" -mca btl_tcp_if_include br0 "$PROGRAM" "$i" "$j" >> "results/"$p"proc_"$j"bcast_"$CSV_FILE"";
                    done
            done
    done