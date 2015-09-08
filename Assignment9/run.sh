#!/bin/bash

FILENAME=bloc.txt


declare -a arr=("a" "b" "c" "d")
declare -a array=()
declare -a secondarray=()
  
    count=${#arr[@]}

    I=0

    while [ $I -lt $count ]; do

        J=$((${I}+1))

	while [ $J -lt $count ]; do	

		counter=0
		index=0
		
		while [ $counter -lt $count ];do

			if [ $counter -ne $J ]; then
			
				if [ $counter -ne $I ]; then
				
					array[$index]=$counter
					
					let index=index+1
				fi
			
			fi

			let counter=counter+1
		done

				
		echo OPER pickup\(${arr[$I]},${arr[$J]}\) >> ${FILENAME}
		echo precond: clear\(${arr[$I]}\) on\(${arr[$I]},${arr[$J]}\) gripper_empty\(\) >> ${FILENAME}
		echo addlist: holding\(${arr[$I]}\) clear\(${arr[$J]}\) >> ${FILENAME}
		echo dellist: clear\(${arr[$I]}\) on\(${arr[$I]},${arr[$J]}\) gripper_empty\(\) >> ${FILENAME}
		echo conflict: on\(${arr[$J]},${arr[$I]}\) on\(${arr[$I]},${arr[$J]}\) on\(${arr[array[0]]},${arr[$I]}\) on\(${arr[$I]},${arr[array[0]]}\) on\(${arr[array[1]]},${arr[$I]}\) on\(${arr[$I]},${arr[array[1]]}\) on\(${arr[$I]},table\) holding\(${arr[$J]}\) holding\(${arr[array[0]]}\) holding\(${arr[array[1]]}\) >> ${FILENAME}
		echo END >> ${FILENAME}

		echo >> ${FILENAME}

		echo OPER pickup\(${arr[$J]},${arr[$I]}\) >> ${FILENAME}
		echo precond: clear\(${arr[$J]}\) on\(${arr[$J]},${arr[$I]}\) gripper_empty\(\) >> ${FILENAME}
		echo addlist: holding\(${arr[$J]}\) clear\(${arr[$I]}\) >> ${FILENAME}
		echo dellist: clear\(${arr[$J]}\) on\(${arr[$J]},${arr[$I]}\) gripper_empty\(\) >> ${FILENAME}
		echo conflict: on\(${arr[$I]},${arr[$J]}\) on\(${arr[$J]},${arr[$I]}\) on\(${arr[array[0]]},${arr[$J]}\) on\(${arr[$J]},${arr[array[0]]}\) on\(${arr[array[1]]},${arr[$J]}\) on\(${arr[$J]},${arr[array[1]]}\) on\(${arr[$J]},table\) holding\(${arr[$I]}\) holding\(${arr[array[0]]}\) holding\(${arr[array[1]]}\) >> ${FILENAME}
		echo END >> ${FILENAME}

		echo >> ${FILENAME}

		echo OPER puton\(${arr[$I]},${arr[$J]}\) >> ${FILENAME}
		echo precond: clear\(${arr[$J]}\) holding\(${arr[$I]}\) >> ${FILENAME}
		echo addlist: on\(${arr[$I]},${arr[$J]}\) clear\(${arr[$I]}\) gripper_empty\(\) >> ${FILENAME}
		echo dellist: clear\(${arr[$J]}\) holding\(${arr[$I]}\) >> ${FILENAME}
		echo conflict: holding\(${arr[$I]}\) holding\(${arr[$J]}\) holding\(${arr[array[0]]}\) holding\(${arr[array[1]]}\) on\(${arr[$J]},${arr[$I]}\) on\(${arr[array[0]]},${arr[$I]}\) on\(${arr[array[1]]},${arr[$I]}\) on\(${arr[$I]},table\) on\(${arr[array[0]]},${arr[$J]}\) on\(${arr[array[1]]},${arr[$J]}\) on\(${arr[$I]},${arr[array[0]]}\) on\(${arr[$I]},${arr[array[1]]}\) >> ${FILENAME}
		echo END >> ${FILENAME}

		echo >> ${FILENAME}

		echo OPER puton\(${arr[$J]},${arr[$I]}\) >> ${FILENAME}
		echo precond: clear\(${arr[$I]}\) holding\(${arr[$J]}\) >> ${FILENAME}
		echo addlist: on\(${arr[$J]},${arr[$I]}\) clear\(${arr[$J]}\) gripper_empty\(\) >> ${FILENAME}
		echo dellist: clear\(${arr[$I]}\) holding\(${arr[$J]}\) >> ${FILENAME}
		echo conflict: holding\(${arr[$J]}\) holding\(${arr[$I]}\) holding\(${arr[array[0]]}\) holding\(${arr[array[1]]}\) on\(${arr[$I]},${arr[$J]}\) on\(${arr[array[0]]},${arr[$J]}\) on\(${arr[array[1]]},${arr[$J]}\) on\(${arr[$J]},table\) on\(${arr[array[0]]},${arr[$I]}\) on\(${arr[array[1]]},${arr[$I]}\) on\(${arr[$J]},${arr[array[0]]}\) on\(${arr[$J]},${arr[array[1]]}\) >> ${FILENAME}
		echo END >> ${FILENAME}


		echo >> ${FILENAME}

		

		
	        let J=J+1
	done

	counter=0
	index=0
		
	while [ $counter -lt $count ];do

		if [ $counter -ne $I ]; then
			
				
			secondarray[$index]=$counter
					
			let index=index+1
		
		fi
			
	
		let counter=counter+1

	done

	echo OPER pickup\(${arr[$I]},table\) >> ${FILENAME}
	echo precond: clear\(${arr[$I]}\) on\(${arr[$I]},table\) gripper_empty\(\) >> ${FILENAME}
	echo addlist: holding\(${arr[$I]}\) >> ${FILENAME}
	echo dellist: clear\(${arr[$I]}\) on\(${arr[$I]},table\) gripper_empty\(\) >> ${FILENAME}
	echo conflict: on\(${arr[$I]},table\) on\(${arr[secondarray[0]]},${arr[$I]}\) on\(${arr[$I]},${arr[secondarray[0]]}\) on\(${arr[secondarray[1]]},${arr[$I]}\) on\(${arr[$I]},${arr[secondarray[1]]}\) on\(${arr[secondarray[2]]},${arr[$I]}\) on\(${arr[$I]},${arr[secondarray[2]]}\) holding\(${arr[secondarray[0]]}\) holding\(${arr[secondarray[1]]}\) holding\(${arr[secondarray[2]]}\) >> ${FILENAME}
	echo END >> ${FILENAME}

	echo >> ${FILENAME}

	echo OPER puton\(${arr[$I]},table\) >> ${FILENAME}
	echo precond: holding\(${arr[$I]}\) >> ${FILENAME}
	echo addlist: on\(${arr[$I]},table\) clear\(${arr[$I]}\) gripper_empty\(\) >> ${FILENAME}
	echo dellist: holding\(${arr[$I]}\) >> ${FILENAME}
	echo conflict: holding\(${arr[$I]}\) holding\(${arr[secondarray[0]]}\) holding\(${arr[secondarray[1]]}\) holding\(${arr[secondarray[2]]}\) on\(${arr[secondarray[0]]},${arr[$I]}\) on\(${arr[secondarray[1]]},${arr[$I]}\) on\(${arr[secondarray[2]]},${arr[$I]}\) on\(${arr[$I]},${arr[secondarray[0]]}\) on\(${arr[$I]},${arr[secondarray[1]]}\) on\(${arr[$I]},${arr[secondarray[2]]}\) >> ${FILENAME}
	echo END >> ${FILENAME}

	echo >> ${FILENAME}
	
	let I=I+1

    done
