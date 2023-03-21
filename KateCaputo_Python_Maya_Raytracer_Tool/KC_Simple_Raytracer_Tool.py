import maya.cmds as cmds
import math
from vec3 import Vector3
from dataclasses import dataclass
from functools import partial
    
# a default file path, customized for my own computer
# users are able to input their own filepath and name in the tool
defaultFP = '/Users/katiecaputo/Desktop/' 

# simple material and derived Lambertian classes to create a solid color material for the triangle
class Material(object):
    def __init__(self):
        self.attn = Vector3(0.0, 0.0, 0.0)
                 
class Lambertian(Material):  
    def __init__(self, albedo):
        self.albedo = albedo
        Material.__init__(self)  
    def __str__(self):
        return (str(self.albedo))

# a dataclass to record information when a ray hits a triangle object
@dataclass
class HitRec:
    p: Vector3
    normal: Vector3
    t: float
    mat: Material
    
# a class to define rays    
class Ray:
    def __init__(self, origin, direction):
        self.origin = origin
        self.direction = direction
    
    # given a value t, returns the location on the ray at 't' distance from its origin   
    def at(self, t):
        return self.origin + (t * self.direction)
    
    # obtains the color of the triangle if the ray hits one, background if none   
    def ray_color(self,tri):
        recMtl = Lambertian(Vector3(.5,.5,.5))
        t = tri.hit(self, 0.0, 10000.0, recMtl)
        
        if t > 0:
            return tri.mat.albedo
        
        unit_dir = (self.direction).unit_vector()
        t = 0.5 * (unit_dir.y + 1.0)
        x = (1.0-t) * Vector3(1.0, 1.0, 1.0)
        y = t*Vector3(0.5, 0.7, 1.0)
        return x + y

# adjusts the normals of the triangle
def convertNorm(v1, v2, v3):
    AB = v2 - v1
    BC = v3 - v2
    return AB.cross(BC).unit_vector()

# a class to represent Triangle objects    
class Triangle:
     def __init__(self, pt1, pt2, pt3, mat):
         self.pt1 = pt1
         self.pt2 = pt2
         self.pt3 = pt3
         self.mat = mat
         self.norm = convertNorm(pt1, pt2, pt3)
     
     # given a ray, determines if the ray hits the triangle
     def hit(self, ray1, t_min, t_max, rec):     
         # is the plane parallel to the ray?
         u = abs((self.norm).dot(self.pt1))
         if u < -(2**31):
             return False
            
         # distance from origin to plane
         D = (self.norm).dot(self.pt1)
         
         if (self.norm).dot(ray1.direction) == 0:
             return False
             
         # location along ray, r, where it intersects the plane
         t = - ((self.norm).dot(ray1.origin) + D) / (self.norm).dot(ray1.direction)
             
         
         if t < t_min and t_max < t:
             t = ((self.norm).dot(ray1.origin) + D) / (self.norm).dot(ray1.direction)
             if t < t_min and t_max < t:
                 return False
         
         # this is the point somewhere on the ray, r        
         pointOfPlaneIntersection = ray1.at(t)
         
         # inside out test
         edge1 = self.pt2 - self.pt1
         edge2 = self.pt3 - self.pt2
         edge3 = self.pt1 - self.pt3
         c1 = pointOfPlaneIntersection - self.pt1
         c2 = pointOfPlaneIntersection - self.pt2
         c3 = pointOfPlaneIntersection - self.pt3
         
         if ((self.norm).dot(edge1.cross(c1)) > 0) and ((self.norm).dot(edge2.cross(c2)) > 0) and ((self.norm).dot(edge3.cross(c3)) > 0):
             # update recorded info
             rec.mat = self.mat
             rec.t = t
             rec.p = ray1.at(t)
             
             # set face normal
             front_face = (ray1.direction).dot(self.norm) < 0
             if front_face:
                 rec.norm = self.norm
             else:
                 rec.norm = (self.norm).makeNeg()
             
             return True
         else:
             return False
         
         return True

# returns a string representation of an RGB color    
def write_color(pixel_color, samples_per_pixel):
    return str(round(255.999 * pixel_color.x) -1) + " " + str(round(255.999 * pixel_color.y) -1) + " " + str(round(255.999 * pixel_color.z - 1)) + '\n'

# obtains all the cameras in the scene, excluding default cams (persp, top, side, front)
listAllCamShapes = []
def getAllCams():
    allCams = cmds.ls(type=('camera'), l=True)
    
    defaultCams = [camera for camera in allCams if cmds.camera(cmds.listRelatives(camera, parent=True)[0], startupCamera=True, q=True)]
    otherCams = list(set(allCams) - set(defaultCams))
       
    listAllCamTransforms = cmds.listRelatives(otherCams, p=True)
    
    for a in listAllCamTransforms:
        shapeObj = cmds.listRelatives(a, c=True)
        listAllCamShapes.append(shapeObj[0])
        

    return listAllCamShapes

# a class to represent a Camera object    
class Camera:
    def __init__(self, cShapeString):
        # image will not render if there are no Cameras (other than default) in the scene
        if len(listAllCamShapes) == 0:
            cmds.error("No Cameras in the Scene")
        
        cameraShape = listAllCamShapes[0]
        
        for c in listAllCamShapes:
            if cShapeString == str(c):
                cameraShape = c
        
        # obtains the location of the selected camera
        eyeList = cmds.camera(cameraShape, q=True, position=True)
        eye = Vector3(eyeList[0], eyeList[1], eyeList[2])
        
        self.origin = eye
        view_h = 2.0
        aspect = (16.0 / 9.0)
        view_w = aspect * view_h
        focal_dist = cmds.camera(cameraShape, fd=True, q=True)
        
        self.horizontal = Vector3(view_w, 0.0, 0.0)
        self.vertical = Vector3(0.0, view_h, 0.0)
        self.llc = self.origin - (self.horizontal/2) - (self.vertical/2) - Vector3(0.0, 0.0, focal_dist)        
        
# writes a PPM file of the rendered image        
def writePPM(w, h, fileName, spp, cam, filePath):
    
    # initializes the camera
    c = Camera(cam)
    
    # opens a file to write to
    try:
        fileRef = open(filePath + fileName + '.ppm', 'w')
    except Exception as e:
        print("Could not open file")
    
    # PPM Header
    newLine = 'P3\n' + str(w) + ' ' + str(h) + '\n255\n'
    fileRef.write(newLine)
      

    # Single Triangle Data
    tri1 = Vector3(.3, .3, .4)
    tri2 = Vector3(-.5, .5, .5)
    tri3 = Vector3(0, 0, .5)
    lamMat = Lambertian(Vector3(1.0, 0.0, 0.0))
    tri = Triangle(tri1, tri2, tri3, lamMat)
    
    #PPM Data
    for j in reversed(range(0, (h))):
        for i in range(0, w):
            u = i/(w-1)
            v = j/(h-1)
            
            r = Ray(c.origin, c.llc + u*c.horizontal + v*c.vertical - c.origin)
            
            pixel_color = r.ray_color(tri)
            fileRef.write(write_color(pixel_color, spp))
            
#---INFO FOR CREATING GUI-----
def queryInputs(*args):
    totalNumRays = 0
    w = cmds.intField(args[1], q=True, v = 1)
    h = cmds.intField(args[3], q=True, v = 1)
    fn = cmds.textField(args[5], q=True, tx=1)
    spp = cmds.intField(args[7], q=True, v=1)
    c = cmds.optionMenu(args[8], q=True, v=1)
    fp = cmds.textField(args[10], q=True, tx=1)
    
    # if the user does not input file path or file name, use default values
    if fp == "":
        fp = defaultFP
    if fn == "":
        fn = "newFile"
        
    writePPM(w, h, fn, spp, c, fp)
    
    print("--------------------------------------------------------------------------------------------------")
    print("-----------------------------------------------DONE-----------------------------------------------")
    print("--------------------------------------------------------------------------------------------------")
  
# opens the image from filepath location in Maya image window
def loadImageWindow(*args):
    window = cmds.window('Rendered Image')
    cmds.paneLayout()
    
    fn = cmds.textField(args[1], q=True, tx=1)
    fp = cmds.textField(args[3], q=True, tx=1)
    if fp == "":
        fp = defaultFP
    if fn == "":
        fn = "newFile"
    cmds.image(image = fp + fn + '.ppm')
    cmds.showWindow(window)

    
def runRaytraceTool():
    # creates window for tool
    if cmds.window('raytraceWindow', ex=True):
        cmds.deleteUI('raytraceWindow', window=True)

    cmds.window('raytraceWindow', title='Raytracer', sizeable=False, resizeToFitChildren=True, w=180, h=150)
    mainLayout = cmds.columnLayout()
    
    # field for image width
    cmds.rowLayout(numberOfColumns=2, columnWidth2=[150,150])
    label_01 = cmds.text( label = 'image width:', align = 'left')
    int_01 = imageWidth = cmds.intField( value = 300)
    cmds.setParent('..')
    
    # field for image height
    cmds.rowLayout(numberOfColumns=2, columnWidth2=[150,150])
    label_02 = cmds.text( label = 'image height:', align = 'left')
    int_02 = imageHeight = cmds.intField( value = 170)
    cmds.setParent('..')
    
    # field for output file path
    cmds.rowLayout(numberOfColumns=2, columnWidth2=[150,150])
    label_06 = cmds.text( label = 'output file location:', align = 'left')
    text_02 = filePathStart = cmds.textField('tf2', ed=True, tx='/Users/katiecaputo/Desktop/')
    cmds.setParent('..')
    
    # field for output file name
    cmds.rowLayout(numberOfColumns=2, columnWidth2=[150,150])
    label_03 = cmds.text( label = 'output file name:', align = 'left')
    text_01 = fileName = cmds.textField('tf1', ed=True)
    cmds.setParent('..')
    
    # field for samples per pixel
    cmds.rowLayout(numberOfColumns=2, columnWidth2=[150,150])
    label_04 = cmds.text( label = 'antialiasing samples/pp:', align = 'left')
    int_03 = samples_per_pixel = cmds.intField( value = 10)
    cmds.setParent('..')
    
    # drop down menu to select camera in scene to use when rendering
    cmds.rowLayout(numberOfColumns=1)
    listCams = getAllCams()
    label_05 = cmds.optionMenu(label="render cam")
    for c in listCams:
        onlyCamName = str(c).replace("Shape", "")
        cmds.menuItem(label=onlyCamName)
    cmds.setParent('..')
    
    # buttons for the 2 main functionalities of tool
    cmds.rowLayout(numberOfColumns=2, columnWidth2=[150,150])
    cmds.button( label = 'Apply & Render', command = partial(queryInputs, label_01, int_01, label_02, int_02, label_03, text_01, label_04, int_03, label_05, label_06, text_02) )

    cmds.button(label='Open Image', command=partial(loadImageWindow, label_03, text_01, label_06, text_02))
    cmds.setParent('..')

    cmds.showWindow()
    

runRaytraceTool()