import threading
import multiprocessing as mp
import time
from datetime import datetime, timezone, timedelta

time_zone = timezone(timedelta(hours = +8))
input_name = '' # input file name
shared_arr = [] # inplace change values for task 1, 2, 4

def bubble_sort(front, end):
    global shared_arr
    
    subarr_size = end - front + 1

    for i in range(subarr_size):
        for j in range(front, end, 1):

            if shared_arr[j] > shared_arr[j+1] :
                tmp = shared_arr[j] ;
                shared_arr[j] = shared_arr[j+1] ;
                shared_arr[j+1] = tmp ;

                    
        end -= 1

    
# bubble_sort()

def p_bubble_sort(front, end, pshared_arr):
    
    subarr_size = end - front + 1
    subarr = pshared_arr[front:end+1]

    for i in range(subarr_size):
        for j in range(subarr_size - 1 - i):

            if subarr[j] > subarr[j+1] :
                tmp = subarr[j] ;
                subarr[j] = subarr[j+1] ;
                subarr[j+1] = tmp ;

        end -= 1

    return subarr
    
# p_bubble_sort()

def merge_arr(front, mid, end):
    global shared_arr
    
    left_arr = shared_arr[front:mid+1]
    right_arr = shared_arr[mid+1:end+1]

    left_idx = 0
    right_idx = 0
    i = front ;

    while left_idx < len(left_arr) and right_idx < len(right_arr):
    
        if left_arr[left_idx] < right_arr[right_idx]:
            shared_arr[i] = left_arr[left_idx]
            left_idx += 1 
        
        else:
            shared_arr[i] = right_arr[right_idx]
            right_idx += 1

        i += 1 
    

    while left_idx < len(left_arr):
        shared_arr[i] = left_arr[left_idx]
        i += 1
        left_idx += 1

    while right_idx < len(right_arr):
        shared_arr[i] = right_arr[right_idx]
        i += 1
        right_idx += 1

# merge_arr()

def p_merge_arr(front, mid, end, pshared_arr):

    left_arr = pshared_arr[front:mid+1]
    right_arr = pshared_arr[mid+1:end+1]
    res_arr = [0] * (end - front + 1)
    
    left_idx = 0
    right_idx = 0
    i = 0 ;

    while left_idx < len(left_arr) and right_idx < len(right_arr):
    
        if left_arr[left_idx] < right_arr[right_idx]:
            res_arr[i] = left_arr[left_idx]
            left_idx += 1 
        
        else:
            res_arr[i] = right_arr[right_idx]
            right_idx += 1

        i += 1 
    

    while left_idx < len(left_arr):
        res_arr[i] = left_arr[left_idx]
        i += 1
        left_idx += 1

    while right_idx < len(right_arr):
        res_arr[i] = right_arr[right_idx]
        i += 1
        right_idx += 1
    
    return res_arr

# p_merge_arr()

def read_file():
    global input_name
    arr = []

    while True:
        try:
            input_name = input('file name([c]cancel):')
            
            if input_name == 'c':
                break 
            
            with open(input_name) as file:
                for num in file.readlines():
                    if num != None:
                        num = num.split('\n')[0]
                        
                        try:
                            arr.append(int(num))
                        except: # not a integer
                            pass
            
            print('### read successfully!')
            break
            
        except:
            print('### the file does not exist.')
            
    return arr

# read_file()

def write_file(task_no, cpu_time):
    global input_name, shared_arr
    
    start_write_time = str(datetime.now(time_zone).isoformat(' '))
    input_name = input_name.split('.txt')[0]
    filename = input_name + '_output' + str(task_no) + '.txt'
    
    print('### sorted, start to write.')
    with open(filename, 'w') as file:
        file.write('Sort :\n')
        
        for num in shared_arr:
            msg = str(num) + '\n'
            file.write(msg)
        
        msg = 'CPU Time: ' + str(cpu_time) + ' s\n'
        file.write(msg)
        msg = 'Output Time: ' + start_write_time
        file.write(msg)
        print('### write successfully!')

# write_file()

# when groupping is 0 from task 2, 3 ,4, change the task no. from 1 to original task 
def task_1(modified_mode): 
    global shared_arr
    
    print('### start to sort.')
    start_time = time.time()
    bubble_sort(0, len(shared_arr) - 1)
    end_time = time.time()
    exec_time = end_time - start_time
    
    write_file(modified_mode, exec_time)

# task_1

def task_2():
    global shared_arr
    thread_num = divide_groups()
    
    while thread_num > len(shared_arr) // 2:  # group numbers must more than half of the arr size
        print('### can not grouping since groups is more than input numbers!')
        thread_num = divide_groups()
        
    if thread_num == 0: # do not grouping
        return False
    
    group_elements = get_groups_elements_size(len(shared_arr), thread_num)

    threads = []
    begin_idx = 0
    end_idx = 0
    
    start_time = time.time()
    print('### start to sort.')
    # sort each group
    for i in range(thread_num):
        end_idx = begin_idx + group_elements - 1 

        if end_idx > len(shared_arr)-1 : # already achieve last element
            end_idx = len(shared_arr)-1
        
        t = threading.Thread(target = bubble_sort, args = (begin_idx, end_idx))
        threads.append(t)

        begin_idx = end_idx + 1 
    
    for t in threads:
        t.start()

    for t in threads:
        t.join()
        
    # merge groups
    threads.clear() # initialize list
    groups_num = thread_num 
    remain_groups = []
    remain_group_start_idx = -1 
    cur_group_elements = group_elements # record how many elements are contained in a group
    begin_idx = 0
    end_idx = begin_idx + cur_group_elements - 1 

    while groups_num > 1:
    
        if groups_num % 2 != 0: # odd, keep the last remain group
            groups_num -= 1
            remain_group_start_idx = groups_num * cur_group_elements
            last_group = shared_arr[remain_group_start_idx: ]
            
            if len(last_group) != 0:
                remain_groups.append(last_group)
            
            shared_arr = shared_arr[ :remain_group_start_idx]
            
        
        cur_group_elements *= 2
        begin_idx = 0
        end_idx = begin_idx + cur_group_elements - 1

        this_time_groups_num = groups_num // 2 # merge will minus 1 group

        for i in range(this_time_groups_num):
            t = threading.Thread(target = merge_arr,
                                args = (begin_idx, begin_idx + (end_idx - begin_idx) // 2, end_idx))
            threads.append(t)
            
            begin_idx = end_idx + 1
            end_idx = begin_idx + cur_group_elements - 1
            groups_num -= 1
        
        for t in threads:
            t.start()

        for t in threads:
            t.join()
        
        threads.clear() # initialize list

    # end while
    
    # merge all remain
    for remain in remain_groups:
        mid = len(shared_arr) - 1
    
        for num in remain:
            shared_arr.append(num)
            
        merge_arr(0, mid, len(shared_arr) - 1)
    
    end_time = time.time()
    exec_time = end_time - start_time
    write_file(2, exec_time)
    
    return True
    
# task_2()

def task_3(pshared_arr):
    global shared_arr # for normal merge and write file
    
    print('### core: ', mp.cpu_count())

    process_num = divide_groups()
    
    while process_num > len(pshared_arr) // 2:  # group numbers must more than half of the arr size
        print('### can not grouping since groups is more than input numbers!')
        process_num = divide_groups() 
        
    if process_num == 0: # do not grouping
        return False
    
    group_elements = get_groups_elements_size(len(pshared_arr), process_num)

    results = []
    begin_idx = 0
    end_idx = 0
    
    start_time = time.time()
    print('### start to sort.')
    
    pool = mp.Pool(processes = process_num)
    # sort each group
    for i in range(process_num):
        end_idx = begin_idx + group_elements - 1 

        if end_idx > len(pshared_arr) - 1 : # already achieve last element
            end_idx = len(pshared_arr) - 1

        results.append(pool.apply_async(p_bubble_sort, args = (begin_idx, end_idx, pshared_arr)))

        begin_idx = end_idx + 1 

    pool.close()
    pool.join()
    
    # collect result from child proccesses
    pshared_arr.clear()
    for res in results:
        pshared_arr.extend(res.get())
    
    results.clear()
    
    # merge groups
    groups_num = process_num 
    remain_groups = []
    remain_group_start_idx = -1 
    cur_group_elements = group_elements # record how many elements are contained in a group
    begin_idx = 0
    end_idx = begin_idx + cur_group_elements - 1 

    while groups_num > 1:
    
        if groups_num % 2 != 0: # odd, keep the last remain group
            groups_num -= 1
            remain_group_start_idx = groups_num * cur_group_elements
            last_group = pshared_arr[remain_group_start_idx: ]
            
            if len(last_group) != 0:
                remain_groups.append(last_group)
            
            pshared_arr = pshared_arr[ :remain_group_start_idx]
        
        cur_group_elements *= 2
        begin_idx = 0
        end_idx = begin_idx + cur_group_elements - 1

        this_time_groups_num = groups_num // 2 # merge will minus 1 group
        pool = mp.Pool(processes = this_time_groups_num)

        for i in range(this_time_groups_num):
            results.append(pool.apply_async(p_merge_arr, args = (begin_idx, begin_idx + (end_idx-begin_idx) // 2, end_idx, pshared_arr)))

            begin_idx = end_idx + 1
            end_idx = begin_idx + cur_group_elements - 1
            groups_num -= 1

        pool.close()
        pool.join()
        
        # collect result from child proccesses
        pshared_arr.clear()
        for res in results:
            pshared_arr.extend(res.get())
    
        results.clear() # initialize list

    # end while
    
    # merge all remain
    shared_arr = pshared_arr
    for remain in remain_groups:
        mid = len(shared_arr) - 1
        
        for num in remain:
            shared_arr.append(num)
            
        merge_arr(0, mid, len(shared_arr) - 1)
        
    
    end_time = time.time()
    exec_time = end_time - start_time
    write_file(3, exec_time)
    
    return True
    
# task_3()

def task_4():
    global shared_arr
    groups = divide_groups()
    
    while groups > len(shared_arr) // 2:  # group numbers must more than half of the arr size
        print('### can not grouping since groups is more than input numbers!')
        groups = divide_groups() 
    
    if groups == 0: # do not grouping
        return False
    
    group_elements = get_groups_elements_size(len(shared_arr), groups)

    begin_idx = 0
    end_idx = 0
    
    start_time = time.time()
    print('### start to sort.')
    # sort each group
    for i in range(groups):
        end_idx = begin_idx + group_elements - 1 

        if end_idx > len(shared_arr) - 1 : # already achieve last element
            end_idx = len(shared_arr) - 1
        
        bubble_sort(begin_idx, end_idx)

        begin_idx = end_idx + 1 

        
    # merge groups
    groups_num = groups 
    remain_groups = []
    remain_group_start_idx = -1 
    cur_group_elements = group_elements # record how many elements are contained in a group
    begin_idx = 0
    end_idx = begin_idx + cur_group_elements - 1 

    while groups_num > 1:
    
        if groups_num % 2 != 0: # odd, keep the last remain group
            groups_num -= 1
            remain_group_start_idx = groups_num * cur_group_elements
            last_group = shared_arr[remain_group_start_idx: ]
            
            if len(last_group) != 0:
                remain_groups.append(last_group)
            
            shared_arr = shared_arr[ :remain_group_start_idx]
            
        
        cur_group_elements *= 2
        begin_idx = 0
        end_idx = begin_idx + cur_group_elements - 1

        this_time_groups_num = groups_num // 2 # merge will minus 1 group

        for i in range(this_time_groups_num):
            merge_arr(begin_idx, begin_idx + (end_idx-begin_idx)//2, end_idx)
            
            begin_idx = end_idx + 1
            end_idx = begin_idx + cur_group_elements - 1
            groups_num -= 1

    # end while
    
    # merge all remain
    for remain in remain_groups:
        mid = len(shared_arr) - 1
    
        for num in remain:
            shared_arr.append(num)
            
        merge_arr(0, mid, len(shared_arr) - 1)
    
    end_time = time.time()
    exec_time = end_time - start_time
    write_file(4, exec_time)
    
    return True
    
# task_4()

def divide_groups():
    
    while True:
        try:
            thread_num = int(input('### please enter a number for k groups:'))
            break 
        except:
            print('### not a integer, please enter again.')
    
    return thread_num 
    
# divide_groups()

def get_groups_elements_size(arr_size, group_num):

    group_elements_size = 0 

    if arr_size % group_num == 0:
        group_elements_size = arr_size // group_num 
    else:
        group_elements_size = (arr_size // group_num) + 1 

    return group_elements_size
# get_groups_elements_size()

if __name__ == '__main__':
    base_arr = read_file()
    
    while base_arr:
        print('*********************************')
        print('1. task 1 (normal bubble_sort)')
        print('2. task 2 (threads bubble_sort + merge)')
        print('3. task 3 (processes bubble_sort + merge)')
        print('4. task 4 (bubble_sort + merge)')
        print('any. quit')
        print('*********************************')
        
        choice = input('please enter a choice:')
    
        if choice == '1':
            shared_arr = base_arr
            task_1(1)
        
        elif choice == '2':
            shared_arr = base_arr
            if not task_2():
                task_1(2) # 'cuz grouping is 0 from task 2, modify file number
    
        elif choice == '3':
            pshared_arr = base_arr
            if not task_3(pshared_arr):
                task_1(3) # modify file number
            
        elif choice == '4':
            shared_arr = base_arr
            if not task_4():
                task_1(4) # modify file number
        
        else:
            break 
    
    if base_arr == None:
        print('### list has nothing, end the program.')
    
# main()            
