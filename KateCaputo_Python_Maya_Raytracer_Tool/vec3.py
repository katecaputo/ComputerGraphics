import math
import random

def random_in_unit_sphere():
    while(True):
        p = Vector3(round(random.uniform(-1.0, 1.0), 2), round(random.uniform(-1.0, 1.0), 2), round(random.uniform(-1.0, 1.0), 2))
        if p.length_squared() >= 1:
            continue
        return p
def random_unit_vector():
    return (random_in_unit_sphere()).unit_vector()
def random_in_unit_disk():
    while(True):
        p = Vector3(round(random.uniform(-1.0, 1.0), 2), round(random.uniform(-1.0, 1.0), 2), 0)
        if p.length_squared() >= 1:
            continue
        return p


class Vector3:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
    
    def __add__(self, other):
        return Vector3(self.x + other.x, self.y + other.y, self.z + other.z)
    def __sub__(self, other):
        return Vector3(self.x - other.x, self.y - other.y, self.z - other.z)    
    def vecMul(self, other):
        return Vector3(self.x * other.x, self.y * other.y, self.z * other.z)
    def __mul__(self, other):
        return Vector3(other * self.x, other * self.y, other * self.z)
    def __rmul__(self, other):
        return self * other
    def __truediv__(self, other):
        return (1/other) * self
    
    def __str__(self):
        return str(self.x) + ' ' + str(self.y) + ' ' + str(self.z)
        
    def dot(self, v):
        try:
            return (self.x * v.x) + (self.y * v.y) + (self.z * v.z)
        except:
            print("dot err")
    def cross(u, v):
        a = u.y * v.z - u.z * v.y
        b = u.z * v.x - u.x * v.z
        c = u.x * v.y - u.y * v.x
        return Vector3(a,b,c)
        
    def length_squared(self):
        return (self.x * self.x) + (self.y * self.y) + (self.z * self.z)
    def length(self):
        return math.sqrt(self.length_squared())
    def near_zero(self):
        s = 1e-8
        return (abs(self.x) < s) and (abs(self.y) < s) and (abs(self.z) < s)
    def makeNeg(self):
        return Vector3(-self.x, -self.y, -self.z)
    
    def unit_vector(self):
        return self / self.length()
    
    def random_in_hemisphere(self):
        in_unit_sphere = random_in_unit_sphere()
        if in_unit_sphere.dot(self) > 0.0:
            return in_unit_sphere
        else:
            return in_unit_sphere.makeNeg()
    
    def reflect(self, n):
        return self - 2 * self.dot(n) * n
    def refractVec(self, v_norm, eta_frac):
        #self must be a unit vector
        unit_vec = self.unit_vector()
        cos_theta = min(unit_vec.makeNeg().dot(v_norm), 1.0)
        R_perp_eq = eta_frac * (unit_vec + (cos_theta * v_norm))
        R_parallel_eq = - math.sqrt(abs(1.0 - R_perp_eq.length_squared())) * v_norm
        return R_parallel_eq + R_perp_eq