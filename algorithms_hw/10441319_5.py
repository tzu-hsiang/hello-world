'''
演算法分析基測
學號: 10441319 / 10626305 / 10627255
姓名: 楊宗玲   / 魏子翔   / 蔡怡萱
系級: 財金系   / 資工系   / 資工系
'''

class Bucket_Problem:

    def __init__(self):
        self.bucket_A = 0
        self.bucket_B = 0

        self.capacity_A = 0
        self.capacity_B = 0

        self.target_volume = 0
        
        self.backup_bucket_A = 0
        self.backup_bucket_B = 0
        self.old_volumes_status = [[0,0]]
        self.action_record = []
    
    def set_initarg(self, capacity_A, capacity_B, target):
        self.__init__()
        self.capacity_A = capacity_A
        self.capacity_B = capacity_B
        self.target_volume = target

    def backup_bucket(self):
        self.backup_bucket_A = self.bucket_A
        self.backup_bucket_B = self.bucket_B
    
    def cur_buckets_volumes(self):
        volumes = [self.bucket_A, self.bucket_B]
        return volumes

    def fill_A(self):
        self.backup_bucket()
        self.bucket_A = self.capacity_A
        self.check_bucket('Fill A')

    def fill_B(self):
        self.backup_bucket()
        self.bucket_B = self.capacity_B
        self.check_bucket('Fill B')

    def empty_A(self):
        self.backup_bucket()
        self.bucket_A = 0
        self.check_bucket('Empty A')
 
    def empty_B(self):
        self.backup_bucket()
        self.bucket_B = 0
        self.check_bucket('Empty B')

    # if current volume(A+B) >= capacity(B), pour A to B but A will remain some water
    def A_to_B(self):
        self.backup_bucket()
        self.bucket_A = self.bucket_A + self.bucket_B - self.capacity_B
        self.bucket_B = self.capacity_B
        self.check_bucket('Pour A B')
        
    # if current volume(A+B) >= capacity(A), pour B to A but B will remain some water
    def B_to_A(self):
        self.backup_bucket()
        self.bucket_B = self.bucket_B + self.bucket_A - self.capacity_A
        self.bucket_A = self.capacity_A
        self.check_bucket('Pour B A')

    def All_A_to_B(self):
        self.backup_bucket()
        self.bucket_B += self.bucket_A
        self.bucket_A = 0
        self.check_bucket('Pour A B')
    
    def All_B_to_A(self):
        self.backup_bucket()
        self.bucket_A += self.bucket_B
        self.bucket_B = 0
        self.check_bucket('Pour B A')
    
    # check current status weather does exist or does not
    # if not, check current volume of B is target or not
    def check_bucket(self, action):
   
        if self.cur_buckets_volumes() in self.old_volumes_status:
            # trace back
            self.bucket_A = self.backup_bucket_A
            self.bucket_B = self.backup_bucket_B
 
        else:
            if self.bucket_B == self.target_volume:
                self.old_volumes_status.append(self.cur_buckets_volumes())
                self.action_record.append(action)
                
                for step in self.action_record:
                    print(step)
                
                print("Success")
                self.__init__()
            else:
                self.old_volumes_status.append(self.cur_buckets_volumes())
                self.action_record.append(action)
                self.find_target()
        
        
    def find_target(self):
    
        if self.bucket_A < self.capacity_A:
            self.fill_A()
        
        if self.bucket_B < self.capacity_B:
            self.fill_B()
        
        if self.bucket_A > 0:
            self.empty_A()
        
        if self.bucket_B > 0:
            self.empty_B()
            
        if self.bucket_A + self.bucket_B > self.capacity_B and self.bucket_A > 0:
            self.A_to_B()
            
        if self.bucket_A + self.bucket_B > self.capacity_A and self.bucket_B > 0:
            self.B_to_A()
        
        if self.bucket_A + self.bucket_B <= self.capacity_A and self.bucket_B > 0:
            self.All_B_to_A()
        
        if self.bucket_A + self.bucket_B <= self.capacity_B and self.bucket_A > 0:
            self.All_A_to_B()

 
 
if __name__ == '__main__':
    bp = Bucket_Problem()
    input_list = []
    
    while True:
        bucket_A = 0
        bucket_B = 0
        target = 0
        bucket_A, bucket_B, target = map(int, (input('input a set:').split(' ')))
        
        if bucket_A == 0 and bucket_B == 0 and target == 0:
            break
        
        input_list.append([bucket_A, bucket_B, target])
    
    for input in input_list:
        bp.set_initarg(input[0], input[1], input[2])
        bp.find_target()

        
        