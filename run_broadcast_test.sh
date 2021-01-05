
PROGRAM="broadcast.out"
CSV_NAME="broadcast_test"
HOSTFILE="host_lab004.txt"
PACKET_SIZES=(500)
BROADCAST_VALUES=(0 1)
NUMBER_PROCCESS=(4 8 16)


for p in "${NUMBER_PROCCESS[@]}"
    do
        for j in "${BROADCAST_VALUES[@]}"
            do
                for i in "${PACKET_SIZES[@]}" 
                    do
                        echo "BCAST_TYPE,NPROC,PACKET_SIZE,N_PACKETS,N_BOUNCES,TYPE,NODE,PROCESS,SRC,DST,TAG,COM_TIME,RUNNING_TIME" > "results/"$CSV_NAME"_"$p"proc_"$j"bcast_"$i"size.csv";
                        echo "mpirun -n "$p" --hostfile "$HOSTFILE" -mca btl_tcp_if_include br0 "$PROGRAM" "$i" "$j"";
                        mpirun -n "$p" --hostfile "$HOSTFILE" -mca btl_tcp_if_include br0 "$PROGRAM" "$i" "$j" >> "results/"$CSV_NAME"_"$p"proc_"$j"bcast_"$i"size.csv";
                    done
            done
    done