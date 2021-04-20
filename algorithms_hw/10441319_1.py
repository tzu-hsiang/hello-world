'''
演算法分析基測
學號: 10441319 / 10626305 / 10627255
姓名: 楊宗玲   / 魏子翔   / 蔡怡萱
系級: 財金系   / 資工系   / 資工系
'''

def max_cross_subarray(low, mid, high, array):
    max_low, max_high = low, high
    left_sum = -inf
    sum = 0
    
    for i in range(mid, low-1, -1):
        sum += array[i]
        if sum > left_sum:
            left_sum = sum
            max_low = i
            
    right_sum = -inf
    
    sum = 0
    
    for j in range(mid+1, high + 1):
        sum += array[j]
        if sum > right_sum:
            right_sum = sum
            max_high = j
    
    return (max_low, max_high, left_sum + right_sum)
    
    
def max_subarray(array, low, high):
    mid = (low + high)//2
    if low == high:
        return (low, high, array[low])
    else:
    
        left_low, left_high, left_max = max_subarray(array, low, mid)

        right_low, right_high, right_max = max_subarray(array, mid+1, high)

        mid_low, mid_high, mid_max = max_cross_subarray(low, mid, high, array)

        if left_max > right_max and left_max > mid_max:
            max_sum  = left_max
            sub_low  = left_low
            sub_high = left_high
        elif right_max > left_max and right_max > mid_max:
            max_sum  = right_max
            sub_low  = right_low
            sub_high = left_high
        else:
            max_sum  = mid_max
            sub_low  = mid_low
            sub_high = mid_high
            
    return (sub_low, sub_high, max_sum)

if __name__ == '__main__':

    result_list = []

    length = int(input('please input item numbers:'))
    
    while length != 0:
        arr = list(map(int, input("Enter a multiple value: ").split())) 
        max_value = max_subarray(arr, 0, len(arr)-1)
        result_list.append(max_value)
        
        length = int(input('please input item numbers:'))
    
    for rec in result_list:
        print('Low = ' + str(rec[0]+1) + ' High = ' + str(rec[1]+1) + ' Sum = ' + str(rec[2]) )
        
        
        