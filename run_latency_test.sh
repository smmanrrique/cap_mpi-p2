
PROGRAM="latency.out"
CSV_NAME="latency_test"
HOSTFILE="host_lab004.txt"
PACKET_SIZES=(1 1000 1000000) # 1 byte, 1 kilobyte, 1 megabyte
NUMBER_BOUNCES=(100)
NUMBER_PROCCESS=(4 8)
NUMBER_EXPERIMENTS=10



for p in "${NUMBER_PROCCESS[@]}"
    do
        for j in "${NUMBER_BOUNCES[@]}"
            do
                for i in "${PACKET_SIZES[@]}" 
                    do
                        echo "EXPERIMENT,BCAST_TYPE,NPROC,PACKET_SIZE,N_PACKETS,N_BOUNCES,TYPE,NODE,PROCESS,SRC,DST,TAG,COM_TIME,RUNNING_TIME" > "results/"$CSV_NAME"_"$NUMBER_EXPERIMENTS"experiments_"$p"proc_"$j"bounces_"$i"size.csv";
                        for (( e=1; e<=$NUMBER_EXPERIMENTS; e++ ))
                            do
                                echo "mpirun -n "$p" --hostfile "$HOSTFILE" -mca btl_tcp_if_include br0 "$PROGRAM" "$e" "$i" "$j"";
                                mpirun -n "$p" --hostfile "$HOSTFILE" -mca btl_tcp_if_include br0 "$PROGRAM" "$e" "$i" "$j" >> "results/"$CSV_NAME"_"$NUMBER_EXPERIMENTS"experiments_"$p"proc_"$j"bounces_"$i"size.csv";
                            done
                    done
            done
    done