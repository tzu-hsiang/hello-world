from operator import attrgetter
import copy

class Process:
    def __init__(self, pid, opid, burst_time, arrival_time, priority):
        # type: str
        self.simplified_pid = pid
        self.origin_pid = opid
        self.response_ratio = '' # using str type to skip float comparison
        
        # type: int
        self.burst_time = burst_time
        self.origin_burst_time = burst_time
        self.arrival_time = arrival_time
        self.priority = priority
        self.turnaround_time = 0
        self.waiting_time = 0
        
# end class Process


class PidsTimeInfo:
    def __init__(self, origin_pid, waiting_time, turnaround_time):
        # type int
        self.pid = int(origin_pid)
        self.waiting_time = waiting_time
        self.turnaround_time = turnaround_time
        
# end class PidsTimeInfo


class ResultLog:
    def __init__(self, scheduling_type, exec_log, time_info_log):
        self.scheduling_type = scheduling_type
        self.exec_log = exec_log
        self.time_info_log = time_info_log
        
# end class ResultLog

# output log
output_log = []

# first index of queue
FRONT = 0


class Scheduling:
    def __init__(self, time_slice, input_list):
        self.__job_list = input_list
        self.__ready_queue = []
        self.__time_slice = time_slice
        self.__exec_log = '' # record running process or not in it every second 
    # end __init__()
    
    def __fetch_jobs(self, cur_time):
        
        # process(es) come in, put the job into ready queue
        for job in self.__job_list:
            if job.arrival_time == cur_time:
                self.__ready_queue.append(job)
            
        # renew _job_list
        self.__job_list = [job for job in self.__job_list if job.arrival_time != cur_time]
        
    # end __fetch_jobs()
    
    def __calculate_pid_timeinfo(self, cur_time, specified_pos):
        t_pid = self.__ready_queue[specified_pos].origin_pid
                
        # finish - arrival 
        t_turnaround_time = (cur_time + 1) - self.__ready_queue[specified_pos].arrival_time
        t_waiting_time = t_turnaround_time - self.__ready_queue[specified_pos].origin_burst_time
        one_record = PidsTimeInfo(t_pid, t_waiting_time, t_turnaround_time)
        return one_record
    # end __calculate_pid_timeinfo()
    
    def __fcfs(self):
        time_info_list = []    
        cur_time = 0
        
        while self.__job_list or self.__ready_queue:
            # process(es) come in, put the job into ready queue
            self.__fetch_jobs(cur_time)
            
            # execute front of ready queue
            if self.__ready_queue:
                self.__exec_log += self.__ready_queue[FRONT].simplified_pid
                self.__ready_queue[FRONT].burst_time -= 1
                
                if self.__ready_queue[FRONT].burst_time == 0: # process is terminated
                    one_record = self.__calculate_pid_timeinfo(cur_time, FRONT)
                    time_info_list.append(one_record)
                    
                    del(self.__ready_queue[FRONT]) # delete terminated process
                    
                # end if
            # end if
            
            # ready_queue is empty now
            else:
                self.__exec_log += '-'
                
            # end else
            
            # the cpu time is adding
            cur_time += 1
            
        # end while
        
        result_log = ResultLog('FCFS', self.__exec_log, time_info_list)
        output_log.append(result_log)
    # end __fcfs()
        
    def __rr(self):
        time_info_list = []   
        cur_time = 0
        cur_time_slice = self.__time_slice
        time_out_process = self.__job_list[FRONT]
        
        # if set True, next cpu time will enqueue previous time out process
        re_enqueue_flag = False
        while self.__job_list or self.__ready_queue:
            # process(es) come in, put the job into ready queue
            self.__fetch_jobs(cur_time)
            
            if re_enqueue_flag:
                self.__ready_queue.append(time_out_process)
                re_enqueue_flag = False
            
            # execute front of ready queue
            if self.__ready_queue:
                self.__exec_log += self.__ready_queue[FRONT].simplified_pid
                self.__ready_queue[FRONT].burst_time -= 1
                cur_time_slice -= 1
                
                if self.__ready_queue[FRONT].burst_time == 0: # process is terminated
                    one_record = self.__calculate_pid_timeinfo(cur_time, FRONT)
                    time_info_list.append(one_record)
                    
                    del(self.__ready_queue[FRONT]) # delete terminated process
                    
                    # initialize arrgument for next process
                    cur_time_slice = self.__time_slice
                # end if
                
                if cur_time_slice == 0 and self.__ready_queue: # time out
                    # dequeue and enqueue at next cpu time
                    time_out_process = self.__ready_queue[FRONT]
                    del(self.__ready_queue[FRONT])
                    re_enqueue_flag = True
                    
                    # initialize arrgument for next process
                    cur_time_slice = self.__time_slice
                # end if    
                
            # end if
            # ready_queue is empty now
            else:
                self.__exec_log += '-'
            
            # the cpu time is adding
            cur_time += 1
        # end while
        
        result_log = ResultLog('RR', self.__exec_log, time_info_list)
        output_log.append(result_log)
    # end __rr()
    
    def __SRTF_find_next_exec_pid(self):
        # priority: latest is first
        # 1. remaining cpu burst time
        # 2. arrival time
        # 3. pid
        
        min_burst_time = self.__ready_queue[FRONT].burst_time
        
        for p in self.__ready_queue:
            if p.burst_time < min_burst_time:
                min_burst_time = p.burst_time
                
        min_burst_time_buf = []
        
        for p in self.__ready_queue:
            if p.burst_time == min_burst_time:
                min_burst_time_buf.append(p)
        
        if len(min_burst_time_buf) == 1:
            return min_burst_time_buf[0].simplified_pid
        else:
            # there are more than one process has same remaining cpu burst time
            # to select process, which has least arrival time
            
            min_arrival_time = min_burst_time_buf[FRONT].arrival_time
            
            for p in min_burst_time_buf:
                if p.arrival_time < min_arrival_time:
                    min_arrival_time = p.arrival_time
                    
            min_arrival_time_buf = []
            
            for p in min_burst_time_buf:
                if p.arrival_time == min_arrival_time:
                    min_arrival_time_buf.append(p)
            
            if len(min_arrival_time_buf) == 1:
                return min_arrival_time_buf[0].simplified_pid
            else:
                # there are more than one process has same arrival time
                # to select process, which has least pid
                min_arrival_time_buf.sort(key = attrgetter('simplified_pid'))
                
                return min_arrival_time_buf[0].simplified_pid
            # end else
        # end else     
    # end __SRTF_find_next_exec_pid()                
    
    def __srtf(self):
        time_info_list = []  
        cur_time = 0
        
        while self.__job_list or self.__ready_queue:
            # process(es) come in, put the job into ready queue
            self.__fetch_jobs(cur_time)
            
            # select process to execute by given priority
            if self.__ready_queue:
                next_exec_pid = self.__SRTF_find_next_exec_pid()
            
                # search the process to execute by pid
                for idx in range(len(self.__ready_queue)):
                    if next_exec_pid == self.__ready_queue[idx].simplified_pid:
                        self.__exec_log += self.__ready_queue[idx].simplified_pid
                        self.__ready_queue[idx].burst_time -= 1
                        
                        if self.__ready_queue[idx].burst_time == 0: # process is terminated
                            one_record = self.__calculate_pid_timeinfo(cur_time, idx)
                            time_info_list.append(one_record)
                            del(self.__ready_queue[idx]) # delete terminated process
                            
                        break
                # end for
            
            # ready_queue is empty now
            else:
                self.__exec_log += '-'
            
            # the cpu time is adding
            cur_time += 1
            
        # end while
        
        result_log = ResultLog('SRTF', self.__exec_log, time_info_list)
        output_log.append(result_log)
    # end __srtf()
    
    def __find_min_priority_pid(self):
        results = []
        
        min = self.__ready_queue[FRONT].priority
        for p in self.__ready_queue:
            if p.priority < min:
                min = p.priority
        
        for p in self.__ready_queue:
            if p.priority == min:
                results.append(p.simplified_pid)
        
        return results
    
    def __pprr(self):
        time_info_list = []
        cur_time = 0
        time_slice = self.__time_slice # assign complete time slice for first rr process
        rr_buf = []
        do_RR = False
        preprocess_flag = False
        next_exec_pid = ''
        prev_proc = '' # for a process not in rr state and preemption occur 
        
        while self.__job_list or self.__ready_queue:
            # process(es) come in, put the job into ready queue
            self.__fetch_jobs(cur_time)
            
            # select process to execute by given priority
            if self.__ready_queue:
                prev_proc = next_exec_pid
                min_priority_pids = self.__find_min_priority_pid()
                
                # preemption is occur when normal priority scheduling
                if (not do_RR) and (prev_proc != min_priority_pids[0]):
                    time_slice = self.__time_slice

                if len(min_priority_pids) == 1:
                    if do_RR: # preemption is occur
                        do_RR = False
                        
                        #print(rr_buf, ',' ,preprocess_flag)
                        if not preprocess_flag:
                            preemption_proc_pid = rr_buf[FRONT]
                            rr_buf = rr_buf[1: ]
                            rr_buf.append(preemption_proc_pid)
                            preprocess_flag = False
                        #print(rr_buf)
                        
                        time_slice = self.__time_slice # for next_exec_pid
                    
                    elif time_slice == 0:
                        time_slice = self.__time_slice
                        
                    next_exec_pid = min_priority_pids[0]    
                # end if
                
                # there are more than 1 processes has same priority, do RR
                else:
                    do_RR = True
                    
                    if not rr_buf:
                        rr_buf = min_priority_pids
                        next_exec_pid = min_priority_pids[0]
                    else:
                        next_exec_pid = rr_buf[FRONT]
                        
                   
                # end else
                proc_complete_flag = False

                # search the process to execute by pid
                for idx in range(len(self.__ready_queue)):
                    if next_exec_pid == self.__ready_queue[idx].simplified_pid:
                        self.__exec_log += self.__ready_queue[idx].simplified_pid
                        self.__ready_queue[idx].burst_time -= 1
                        time_slice -= 1
                        
                        if self.__ready_queue[idx].burst_time == 0: # process is terminated
                            one_record = self.__calculate_pid_timeinfo(cur_time, idx)
                            time_info_list.append(one_record)
                            
                            del(self.__ready_queue[idx]) # delete terminated process
                            proc_complete_flag = True
                            
                        break
                # end for
                
                preprocess_flag = False
                
                if do_RR and (time_slice == 0 or proc_complete_flag):

                    # search next rr_cur_exec_pid, for next cpu time
                    for idx in range(len(min_priority_pids)):
                        if rr_buf[FRONT] == min_priority_pids[idx]:
                            
                            # the process is terminated
                            if proc_complete_flag:
                                proc_complete_flag = False
                                rr_buf = rr_buf[1: ]
                            
                            # time out
                            elif time_slice == 0:
                                time_out_pid = rr_buf[FRONT]
                                rr_buf = rr_buf[1: ]
                                rr_buf.append(time_out_pid)
                            
                            time_slice = self.__time_slice # assign complete time slice to new
                            preprocess_flag = True
                            
                            break
                        #  end if
                    # end for
                # end if
                
                # just a process is terminated when using normal priority scheduling
                elif proc_complete_flag: 
                    time_slice = self.__time_slice # assign complete time slice to new
                # end elif
                
                # see as normal priority scheduling
                if do_RR and len(rr_buf) == 1:
                    rr_buf.clear()
                    do_RR = False
                    

            # ready_queue is empty now
            else:
                self.__exec_log += '-'
                
            # the cpu time is adding
            cur_time += 1
            
        # while
        result_log = ResultLog('PPRR', self.__exec_log, time_info_list)
        output_log.append(result_log)
    # end __pprr()
    
    def __HRRN_find_next_exec_pid(self):
        # if there are same value exist in 1, 2 stages, go next stage
        # 1. response ratio (choose largest)
        # 2. arrival time (choose least)
        # 3. pid (choose least)
        
        max_response_ratio = self.__ready_queue[FRONT].response_ratio
        
        for p in self.__ready_queue:
            if p.response_ratio > max_response_ratio:
                max_response_ratio = p.response_ratio
                
        max_response_ratio_buf = []
        
        for p in self.__ready_queue:
            if p.response_ratio == max_response_ratio:
                max_response_ratio_buf.append(p)
        
        if len(max_response_ratio_buf) == 1:
            return max_response_ratio_buf[0].simplified_pid
        else:
            # there are more than one process has same response ratio
            # to select process, which has least arrival time
            
            min_arrival_time = max_response_ratio_buf[FRONT].arrival_time
            
            for p in max_response_ratio_buf:
                if p.arrival_time < min_arrival_time:
                    min_arrival_time = p.arrival_time
                    
            min_arrival_time_buf = []
            
            for p in max_response_ratio_buf:
                if p.arrival_time == min_arrival_time:
                    min_arrival_time_buf.append(p)
            
            if len(min_arrival_time_buf) == 1:
                return min_arrival_time_buf[0].simplified_pid
            else:
                # there are more than one process has same arrival time
                # to select process, which has least pid
                min_arrival_time_buf.sort(key = attrgetter('simplified_pid'))
                
                return min_arrival_time_buf[0].simplified_pid
            # end else
        # end else     
    # end __HRRN_find_next_exec_pid()      
    
    def __calculate_response_ratio(self):
        for p in self.__ready_queue:
            float_ratio = str((p.burst_time + p.waiting_time) / p.burst_time)
                
            # round down ratio to the 1000th digit
            t_list = float_ratio.split('.')
            float_ratio = t_list[0] + '.' + t_list[1][ :3]
           
            p.response_ratio = str(float_ratio)
            
        # end for
    # end __calculate_response_ratio()
    
    def __hrrn(self):
        time_info_list = []
        cur_time = 0
        running_flag = False # for not preemptive
        first_run_flag = True
        
        while self.__job_list or self.__ready_queue:
            # process(es) come in, put the job into ready queue
            self.__fetch_jobs(cur_time)
            
            # calculate response ratio of processes in ready queue every cpu time
            self.__calculate_response_ratio()
            
            if self.__ready_queue:
                # select the process, which has least ratio
                # if there are more than one processes, select it by given priority
                if first_run_flag:
                    next_exec_pid = self.__HRRN_find_next_exec_pid()
                    running_flag = True
                    first_run_flag = False
                
                elif not running_flag:
                    next_exec_pid = self.__HRRN_find_next_exec_pid()
                    running_flag = True
            
                # search the process to execute by pid
                for idx in range(len(self.__ready_queue)):
                    if next_exec_pid == self.__ready_queue[idx].simplified_pid:

                        self.__exec_log += self.__ready_queue[idx].simplified_pid
                        self.__ready_queue[idx].burst_time -= 1
                        
                        if self.__ready_queue[idx].burst_time == 0: # process is terminated
                            one_record = self.__calculate_pid_timeinfo(cur_time, idx)
                            time_info_list.append(one_record)
                            
                            del(self.__ready_queue[idx]) # delete terminated process
                            
                            running_flag = False
                            break
                    # end if
                # end for
                
                for idx in range(len(self.__ready_queue)):
                    # adding waiting time without current running
                    if next_exec_pid != self.__ready_queue[idx].simplified_pid:
                        self.__ready_queue[idx].waiting_time += 1
                        #print(next_exec_pid, ',', self.__ready_queue[idx].simplified_pid, ',', self.__ready_queue[idx].waiting_time)
                        
                # end for        
            # end if

            # ready_queue is empty now
            else:
                self.__exec_log += '-'
            
            # the cpu time is adding
            cur_time += 1
            
        # end while
        
        result_log = ResultLog('HRRN', self.__exec_log, time_info_list)
        output_log.append(result_log)
    # end __hrrn()
        
        
    def FCFS(self):
        self.__fcfs()
    
    def RR(self):
        self.__rr()
    
    def SRTF(self):
        self.__srtf()
        
    def PPRR(self):
        self.__pprr()
        
    def HRRN(self):
        self.__hrrn()
    
# end class Scheduling

def change_pid(pid):
    if pid == '10':
        return 'A'
    elif pid == '11':
        return 'B'
    elif pid == '12':
        return 'C'
    elif pid == '13':
        return 'D'
    elif pid == '14':
        return 'E'    
    elif pid == '15':
        return 'F'
    elif pid == '16':
        return 'G'
    elif pid == '17':
        return 'H'    
    elif pid == '18':
        return 'I'    
    elif pid == '19':
        return 'J'    
    elif pid == '20':
        return 'K'    
    elif pid == '21':
        return 'L'    
    elif pid == '22':
        return 'M'    
    elif pid == '23':
        return 'N'    
    elif pid == '24':
        return 'O'
    elif pid == '25':
        return 'P'    
    elif pid == '26':
        return 'Q'
    elif pid == '27':
        return 'R'
    elif pid == '28':
        return 'S'    
    elif pid == '29':
        return 'T'    
    elif pid == '30':
        return 'U'
    elif pid == '31':
        return 'V'    
    elif pid == '32':
        return 'W'    
    elif pid == '33':
        return 'X'
    elif pid == '34':
        return 'Y'    
    elif pid == '35':
        return 'Z'    
    else:
        return pid
# end change_pid()

def readfile():
    input_list = []
    mode = 0
    time_slice = 0
    input_name = input('input the file name([q]quit):')
    
    while input_name != 'q' or input_name != 'Q':
        try:
            with open(input_name) as file:
                for line in file.readlines():
                    if line != None:
                        line_tmp = line.lstrip().strip('\n').split()
                            
                        if line_tmp:
                            if line_tmp[0].isdigit():
                                if len(line_tmp) == 4:
                                    simplified_pid = change_pid(line_tmp[0])
                                    process = Process(simplified_pid, line_tmp[0], int(line_tmp[1]), int(line_tmp[2]), int(line_tmp[3]))
                                        
                                    input_list.append(process)
                                    
                                else: # first line (len == 2)
                                    mode = int(line_tmp[0])
                                    time_slice = int(line_tmp[1])
                        # end if
                    # end if                
                break
            # end for
        except:
            if input_name == 'q' or input_name == 'Q':
                return 0, 0, None, ''
            else:
                print('the file doesn\'t exist')
                input_name = input('input the file name([q]quit):')
                        
    
    return mode, time_slice, input_list, input_name

# end readfile()

def writefile(scheduling_type, in_file_name):
    out_file_name = 'out_' + in_file_name

    with open(out_file_name, 'w') as f:
        f.write(scheduling_type + '\n')
    
        # execution log
        for onelog in output_log:
                    
            f.write('==' + '%12s' %(onelog.scheduling_type) + '==' + '\n')
            f.write(onelog.exec_log + '\n')
        # end for
        f.write('===========================================================\n\n')
        
        # sort time info records by pid
        for onelog in output_log:
            onelog.time_info_log.sort(key = attrgetter('pid')) # sort by pid
                
        # waiting time header
        f.write('waiting' + '\n')
        f.write('ID' + '\t')
        for idx in range(len(output_log)):
            if idx != len(output_log) - 1:
                f.write(output_log[idx].scheduling_type + '\t')
            else:
                f.write(output_log[idx].scheduling_type + '\n')
                        
        # end for
        f.write('===========================================================\n')
                
        # waiting time log
        for pid_idx in range(len(output_log[0].time_info_log)):
            f.write('%-3s' % output_log[idx].time_info_log[pid_idx].pid + '\t')
                
            for idx in range(len(output_log)):
                    
                if idx != len(output_log) - 1:
                    f.write('%-s' % output_log[idx].time_info_log[pid_idx].waiting_time + '\t')
                else:
                    f.write('%-s' % output_log[idx].time_info_log[pid_idx].waiting_time + '\n')
                    
                            
            # end for
        # end for
        f.write('===========================================================\n\n')
                
        # turnaround time header
        f.write('Turnaround Time' + '\n')
        f.write('ID' + '\t')
        for idx in range(len(output_log)):
            if idx != len(output_log) - 1:
                f.write(output_log[idx].scheduling_type + '\t')
            else:
                f.write(output_log[idx].scheduling_type + '\n')
        # end for
        f.write('===========================================================\n')
        
        # turnaround time log
        for pid_idx in range(len(output_log[0].time_info_log)):
            f.write('%-3s' % output_log[idx].time_info_log[pid_idx].pid + '\t')
                
            for idx in range(len(output_log)):
                    
                if idx != len(output_log) - 1:
                    f.write('%-s' % output_log[idx].time_info_log[pid_idx].turnaround_time + '\t')
                else:
                    f.write('%-s' % output_log[idx].time_info_log[pid_idx].turnaround_time + '\n')
                    
                            
            # end for
        # end for
        f.write('===========================================================\n\n')
        
    # end with open
    
# end writefile()

def prog_exec():
    mode, time_slice, input_list, in_file_name = readfile()
    
    while mode != 0:
        scheduling_type = ''
    
        if mode == 1: # FCFS
            scheduling_type = 'FCFS'
            sorted_list = sorted(input_list, key = attrgetter('simplified_pid')) # sort list by simplified_pid
            scheduling = Scheduling(time_slice, sorted_list)
            scheduling.FCFS()
            del scheduling
            
        
        elif mode == 2: # RR
            scheduling_type = 'RR'
            sorted_list = sorted(input_list, key = attrgetter('simplified_pid')) # sort list by simplified_pid
            scheduling = Scheduling(time_slice, sorted_list)
            scheduling.RR()
            del scheduling
        
        elif mode == 3: # SRTF
            scheduling_type = 'SRTF'
            sorted_list = sorted(input_list, key = attrgetter('simplified_pid')) # sort list by simplified_pid
            scheduling = Scheduling(time_slice, sorted_list)
            scheduling.SRTF()
            del scheduling
        
        elif mode == 4: # PPRR
            scheduling_type = 'PPRR'
            sorted_list = sorted(input_list, key = attrgetter('priority')) # sort list by priority
            scheduling = Scheduling(time_slice, sorted_list)
            scheduling.PPRR()
            del scheduling
        
        elif mode == 5: # HRNN
            scheduling_type = 'HRNN'
            sorted_list = sorted(input_list, key = attrgetter('simplified_pid')) # sort list by simplified_pid
            scheduling = Scheduling(time_slice, sorted_list)
            scheduling.HRNN()
            del scheduling
        
        elif mode == 6: # ALL
            scheduling_type = 'All'
            
            sorted_list = sorted(input_list, key = attrgetter('simplified_pid')) # sort list by simplified_pid
            scheduling = Scheduling(time_slice, copy.deepcopy(sorted_list))
            scheduling.FCFS()
            
            scheduling.__init__(time_slice, copy.deepcopy(sorted_list))
            scheduling.RR()
            
            scheduling.__init__(time_slice, copy.deepcopy(sorted_list))
            scheduling.SRTF()
            
            sorted_list = sorted(input_list, key = attrgetter('priority')) # sort list by priority
            scheduling.__init__(time_slice, copy.deepcopy(sorted_list))
            scheduling.PPRR()
            
            scheduling.__init__(time_slice, copy.deepcopy(sorted_list))
            scheduling.HRRN()
            
            del scheduling
            
        else: # exception
            print('exception: Unknown mode= ', mode)
            
        if mode >= 1 and mode <= 6:
            print('start to write file')
            writefile(scheduling_type, in_file_name)
            print('successed!')
            output_log.clear()
        
        # do prog again until user input a q or Q
        mode, time_slice, input_list, in_file_name = readfile()
        
    # end while


# end prog_exec

if __name__ == '__main__':
    prog_exec()
    
# end main
    
    

    