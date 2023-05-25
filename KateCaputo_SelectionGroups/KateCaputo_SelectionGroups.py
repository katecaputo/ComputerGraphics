import maya.cmds as cmds
import os
from functools import partial


    
    
# a class to represent selection groups
class SelGroup:
    def __init__(self, name):
        self.items = []
        self.name = name
    
    
    # adds objects to a selection group
    def addToGroup(self, objsToAdd):
        if objsToAdd is None:
            cmds.error("no objects in list to add to selection group")
        else:
            for o in objsToAdd:
                self.items.append(o)
    
    
    # remove objects from a selection group
    def removeFromGroup(self, objsToRem):
        if objsToRem is None:
            cmds.error("no objects in list to remove from selection group")
        else:
            for a in objsToRem:
                for b in self.items:
                    if a == b:
                        self.items.remove(b)
                        break  


# a class to represent the entire UI functionality
class myUI:
    
    # ---------------------------- THE MAIN CONTROL SETS WINDOW UI -----------------------------
    def __init__(self):
        
         # represents all the selection groups created so far
        self.allSelGroup = []
        self.allSelGroupNames = []
        
        # if this is the first time the window has been called, creates a blank .txt file
        curFilePath = cmds.file(q=True, sn=True)
        nameOnly, ext = os.path.splitext(curFilePath)
        
        self.winName = 'controlSets'
        self.winWidth = 600
        if cmds.window(self.winName, exists=True):
            cmds.deleteUI(self.winName)
            
        # deletes existing secondary windows
        if cmds.window("createWindow", exists = True):
                cmds.deleteUI("createWindow")
        if cmds.window("editWindow", exists = True):
                cmds.deleteUI("editWindow")
                
        self.window = cmds.window(self.winName, w=self.winWidth, title='Control Sets Window')
        self.colLayout = cmds.columnLayout() 
        
        numColumns = 2
       
        allPanesWidth = [self.winWidth*(1/numColumns) - 30, self.winWidth*(1/numColumns) - 30]
        panesLayout = cmds.rowLayout(w=self.winWidth, numberOfColumns=2, columnWidth2=[300, 300], rowAttach=[(1, 'top', 10), (2, "top", 120)])
        
        innerRowWidth = [allPanesWidth[0], allPanesWidth[0]]
        
        # ---------- panel 1 ----------
        
        column1 = cmds.columnLayout(parent = panesLayout, w=allPanesWidth[0], rowSpacing=10)
        cmds.text( parent = column1, label='my sets', align='center', w=[allPanesWidth[0], allPanesWidth[0]][0])
        
        # UI of sets list
        cmds.rowLayout(numberOfColumns=1, columnWidth2=innerRowWidth)
        self.setsList = cmds.textScrollList(width=innerRowWidth[0], ams=True, h=220)
        cmds.textScrollList(self.setsList, e=True, ra=True)
        cmds.textScrollList(self.setsList, e=True, append= self.allSelGroupNames)
        cmds.setParent(column1)
        
        # UI of buttons on lower left side
        cmds.rowLayout(parent=column1, numberOfColumns=2, columnWidth2=[allPanesWidth[0]*.5,allPanesWidth[0]*.5], ct2 = ['both','both'], co2 = [20,20])
        cmds.button(label = "select set", w = (innerRowWidth[0]/2) - 40, command=partial(self.selectAllInSet))
        cmds.button(label = "key set", w = (innerRowWidth[0]/2) - 40, command=partial(self.keySets))
        cmds.setParent(column1)
        
        
        cmds.rowLayout(parent=column1, numberOfColumns=2, columnWidth2=[allPanesWidth[0]*.5, allPanesWidth[0]*.5], ct2 = ['both','both'], co2 = [20,20])
        cmds.button(label = "delete set", w = (innerRowWidth[0]/2) - 40, command=partial(self.deleteSets))
        cmds.button(label = "duplicate set", w = (innerRowWidth[0]/2) - 40, command=partial(self.duplicateSets))
        cmds.setParent(column1)
        
        cmds.rowLayout(parent=column1, numberOfColumns=3, columnWidth3=[allPanesWidth[0]*.2, allPanesWidth[0]*.5, allPanesWidth[0]*.3], ct3=['right', 'both', 'both'], co3=[5, 5, 5])
        cmds.text(label="rename:")
        self.renameTextField = cmds.textField(ed=True)
        cmds.button(label="save name", command=partial(self.renameSet))
        cmds.setParent(column1)
        

        cmds.setParent(column1)
        
        # ---------- panel 2 ----------
        
        column2 = cmds.columnLayout(parent = panesLayout, w=allPanesWidth[0], rowSpacing=10, cat=['both', 20])
        # UI of 'create new' and 'edit existing' buttons on left side
        cmds.button(label="create new set", parent=column2,align='center', w=[allPanesWidth[0], allPanesWidth[0]][0] - 30, h=40, command=partial(self.launchSecondaryWindow, "create"))
        cmds.button(label="edit selected set", parent=column2,align='center', w=[allPanesWidth[0], allPanesWidth[0]][0] - 30, h=40, command=partial(self.launchSecondaryWindow, "edit"))
        cmds.setParent(column2)
        
        # ------------ bottom -------------------
        
        # UI of 'save' and 'load' buttons at bottom of window
        cmds.setParent(self.colLayout)
        cmds.separator()
        cmds.rowLayout(parent=self.colLayout, numberOfColumns=2, columnWidth2=[300, self.winWidth*.5], ct2 = ['both','both'])
        cmds.button(label='load in sets', height=35, command=partial(self.loadInSets))
        cmds.button(label='apply and close', height=35, command=partial(self.applyClose))
        cmds.setParent(self.colLayout)
        
        
        if not os.path.exists(nameOnly + '_selGroups.txt'):
            fileRef = open(nameOnly + '_selGroups.txt', 'w')
            fileRef.close()
        self.loadInSets()
        
        # launches the UI window
        cmds.showWindow(self.winName)
        cmds.window(self.winName, e=True, width=self.winWidth, height=1)
     
     
    # a function to select all the objects within a given sets     
    def selectAllInSet(self, *args):
        # determines which set is selected in the list
        setsSelected = cmds.textScrollList(self.setsList, q=True, si=True)
        
        cmds.select(clear=True)
        
        # ensures a set is selected; if so, selects all items within that set
        if setsSelected is None:
            cmds.confirmDialog( title='Please choose which set you would like to select', button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')
        else:
            for s in setsSelected:
                ind = self.allSelGroupNames.index(s)
                for i in self.allSelGroup[ind].items:
                    cmds.select(i, add=True)
    
    
    # a function to delete the selected sets            
    def deleteSets(self, *args):
        setsSelected = cmds.textScrollList(self.setsList, q=True, si=True)
        
        # ensures a set is selected; if so, deletes the set from all locations
        if setsSelected is None:
            cmds.confirmDialog( title='Please select a set to delete', button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')
        else:
            for s in setsSelected:
                ind = self.allSelGroupNames.index(s)
                self.allSelGroup.pop(ind)
                self.allSelGroupNames.pop(ind)
        
            # refreshes the list of sets
            cmds.textScrollList(self.setsList, e=True, ra=True)
            cmds.textScrollList(self.setsList, e=True, append=self.allSelGroupNames)
    
    
    # places a keyframe at the current location of the timeslider for all items in the given sets    
    def keySets(self, *args):
        # stores whatever the user had previously selected in a variable
        curSelected = cmds.ls(selection=True)
        
        # selects all items under the given sets and places a keyframe
        self.selectAllInSet()
        cmds.setKeyframe()
        
        # reselects the original items which the user had selected
        cmds.select(clear=True)
        if curSelected is not None:
            for x in curSelected:
                cmds.select(x, add=True)
    
    
    # duplicates the given sets and gives them new names        
    def duplicateSets(self, *args):
        setsSelected = cmds.textScrollList(self.setsList, q=True, si=True)
        
        # ensures a set is selected; if so, duplicates it
        if setsSelected is None:
            cmds.confirmDialog( title='Please select a set to duplicate', button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')
        else:
            for s in setsSelected:
                newName = s + "_copy0"
                i = 0
                # if the new name of the set ('setName_copy0') already exists, increments the copy's name
                while newName in self.allSelGroupNames:
                    newName = newName.replace("copy" + str(i), "copy" + str(i+1))
                    i = i + 1
                
                # creates and adds the new set and name to respective lists
                ind = self.allSelGroupNames.index(s)
                newSelGrp = SelGroup(newName)
                self.allSelGroup.append(newSelGrp)
                self.allSelGroupNames.append(newSelGrp.name)
                indNew = len(self.allSelGroupNames) - 1
                self.allSelGroup[indNew].addToGroup(self.allSelGroup[ind].items)
            
            # refreshes the sets list
            cmds.textScrollList(self.setsList, e=True, ra=True)
            cmds.textScrollList(self.setsList, e=True, append=self.allSelGroupNames)
    
    
    # renames the given set given text input from the UI and user      
    def renameSet(self, *args):
        setsSelected = cmds.textScrollList(self.setsList, q=True, si=True)
        newSetName = cmds.textField(self.renameTextField, q=True, text=True)
        
        # only one set can be selected
        if setsSelected is None:
            cmds.confirmDialog( title='Please select a set to rename', button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')
        elif len(setsSelected) > 1:
            cmds.confirmDialog( title='Too many sets selected', message="Please select only one set to rename", button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')

        # a new set name must be specified, cannot be blank
        elif newSetName == "":
            cmds.confirmDialog( title="Please enter text to rename a set", button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')
        
        # the new name can not be that of an existing set
        elif newSetName in self.allSelGroupNames:
            cmds.confirmDialog( title='Set With Given Name Already Exists', message="Please enter a new, unique set name", button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')  
        else:
            # obtains the index of the group and updates info accordingly
            ind = self.allSelGroupNames.index(setsSelected[0])
            self.allSelGroupNames[ind] = newSetName
            self.allSelGroup[ind].name = newSetName
            
            # refreshes the sets list UI
            cmds.textField(self.renameTextField, e=True, text="")
            cmds.textScrollList(self.setsList, e=True, ra=True)
            cmds.textScrollList(self.setsList, e=True, append=self.allSelGroupNames)
                      
    # ---------------------------- THE SECONDARY UI WINDOWS & FUNCTIONS -----------------------------
    
    # function launches either the 'edit existing' UI or 'create new' UI windows
    def launchSecondaryWindow(self, *args):
        setsSelected = cmds.textScrollList(self.setsList, q=True, si=True)
        
        # initialized the secondary UI and passes the necessary arguments
        if setsSelected is None:
            setsSelected = []
        ui = SecondaryUICreation(args[0], setsSelected, self.allSelGroup, self.allSelGroupNames, self)
    
    
    # refreshes the main sets list in the original UI after working within the secondary UI
    def refreshAfterSecondaryUI(self, *args):
        
        # ensures that if changes were not saved, no edits are finalized
        if not args[0].wasCancelled:   
            # obtains the updated selection group lists
            self.allSelGroup = args[0].updatedSelGroups()
            self.allSelGroupNames = args[0].updatedSelGroupNames()
        
        
            # refreshes the sets list in the original UI, if changes were made
            cmds.textScrollList(self.setsList, e=True, ra=True)
            cmds.textScrollList(self.setsList, e=True, append=self.allSelGroupNames)
             
    # ------------------ SAVE AND LOAD IN ALL SELECTION SETS ------------------ 
    
    # applies the changes and saves the selection set information into a .txt file 
    def applyClose(self, *args):
        # if no selection sets were created, will not save a blank .txt file
        try:
            
            # obtains maya file name and creates a text file in the same directory
            curFilePath = cmds.file(q=True, sn=True)
            nameOnly, ext = os.path.splitext(curFilePath)
            
            open(nameOnly + '_selGroups.txt', 'w').close()
        
            fileRef = open(nameOnly + '_selGroups.txt', 'w')
            
            # writes information for each selection set
            # format:
            #    selectionSetName item1 item2 item3
            #    selectionSetName2 item4 item5
            for x in self.allSelGroup:
                newLine = x.name
                for y in x.items:
                    newLine = newLine + ' ' + y
                newLine = newLine + '\n'
                fileRef.write(newLine)
            
            # provides user with feedback upon successful file save
            cmds.deleteUI(self.winName)  
            cmds.confirmDialog( title='Sucessfully Saved File!', button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')
        
        # catches any errors which arise from file management
        except Exception as e:
            cmds.confirmDialog( title='ERROR: could not save set', button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')

                        
    # a helper function for reading in .txt files with selection group information               
    def processFileLine(self, *args):
        
        # splits the given line into a list, using a space ' ' as delimiter
        lineSplit = args[0].split()
        
        # obtains the first item in the line
        # in a properly formated selection groups .txt file, this will be the name of the selection group
        grpName = lineSplit[0]
        
        # creates a new group of this name
        newGrp = SelGroup(grpName)
        
        
        self.allSelGroup.append(newGrp)
        self.allSelGroupNames.append(grpName)
        # removes the first item in the list (the name), so that the list is made up of the group items only
        lineSplit.pop(0)
        
        # then, adds all of these items into the newly created selection group
        newGrp.addToGroup(lineSplit)
        
            
    # a function for reading a selection group .txt file, and importing them into the current scene      
    def loadInSets(self, *args):        
        
        try:
            # obtains file name of the selection groups .txt file and opens it
            curFilePath = cmds.file(q=True, sn=True)
            nameOnly, ext = os.path.splitext(curFilePath)
            self.allSelGroupNames = []
            self.allSelGroup = []
            
            # opens the file to be read
            fileref = open(nameOnly + '_selGroups.txt', 'r')          

            # reads and processes each line of the .txt file
            line = fileref.readline()
            while(line):
                self.processFileLine(line)
                line = fileref.readline()
            # closes the file   
            fileref.close()
            
            # refreshes the sets list in the original UI, if changes were made
            cmds.textScrollList(self.setsList, e=True, ra=True)
            cmds.textScrollList(self.setsList, e=True, append=self.allSelGroupNames)

        
        except Exception as e:
            cmds.confirmDialog( title='ERROR: could not open .txt file', message="please check directory", button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')
        
        
        
# ---------------------------- HELPER FUNCTIONS ----------------------------
               
# obtains the parent of an object
# used to obtain the transform node of a shape node
def retrieveParent(shapeNode):
    if cmds.objectType(shapeNode, isa='shape'):
        return cmds.listRelatives(shapeNode, p=True)[0]
    else:
        cmds.error("retrieveParent - not a shape node")

# determines whether an object is a group, or another type of transform node        
def isAGroup(node):
    # joints are special cases
    if cmds.objectType(node, isType = 'joint'):
        return False
    children = cmds.listRelatives(node, c=1)
    if children:
        for child in children:
            if not cmds.objectType(child, isType = 'transform'):
                return False
    return True


# ---------------------------- SECONDARY UI ----------------------------

# a class to represent the secondary UI window and functionality
#    this will either be the 'edit sets' window or 'create new sets' window
class SecondaryUICreation:
    
    def __init__(self, type, selection, selGrps, selGrpNames, primaryUI):
        
        # stores the list of selection groups obtained from the primary UI
        self.selGrps = selGrps
        self.selGrpNames = selGrpNames
        self.primaryUI = primaryUI
        
        # was this window cancelled before any saves were made?
        self.wasCancelled = False
        
        # cannot open the 'edit sets' window if more than one or no sets are selected in the primary UI
        if type == 'edit' and len(selection) == 0:
            cmds.confirmDialog( title='Please select a set to edit', button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')
        elif type == 'edit' and len(selection) > 1:
            cmds.confirmDialog( title='Please only select ONE set to edit', button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')
        else:
            if type == 'edit':
                # which selection set is being edited?
                indOfSelection = self.selGrpNames.index(selection[0])
                self.selection = self.selGrps[indOfSelection]

            # sets initial states for checkboxes
            # 0-mesh, 1-lights, 2-cameras, 3-groups, 4-curves, 5-joints, 6-keyed objects
            self.attrArray = [True,True, True, True, True, True, False]
        
        
            # initial window creation
            if type == "create":
                self.winName2 = "createWindow"
            else:
                self.winName2 = "editWindow"
    
            self.winWidth2 = 601
            if cmds.window(self.winName2, exists = True):
                cmds.deleteUI(self.winName2)
            # differentiates between creation and editing windows
            if type == "create":
                self.window2 = cmds.window(self.winName2, w=self.winWidth2, title='Create a New Selection Set')
            else:
                self.window2 = cmds.window(self.winName2, w=self.winWidth2, title='Edit Selection Set')
            self.colLayout2 = cmds.columnLayout()
    
            cAllPanesWidth = [self.winWidth2*.5 - 30, self.winWidth2*.5 - 30]
            cPanesLayout = cmds.rowLayout(w=self.winWidth2, numberOfColumns=3, columnWidth3=[278, 45, 278], rowAttach=[(1, 'top', 10), (2, "top", 120),(3, "top", 45)])
            cInnerRowWidth = [cAllPanesWidth[0], cAllPanesWidth[0]]
        
            # -------------------- panel 1 --------------------
            
            column1 = cmds.columnLayout(w=cAllPanesWidth[0], rowSpacing=10)
            
            # UI for 'outliner'-type list for objects in the scene
            cmds.text(label='select a root and press load', align='center', w=[cAllPanesWidth[0], cAllPanesWidth[0]][0])

            cmds.rowLayout(numberOfColumns=1, columnWidth2=cInnerRowWidth, ct1='both', co1=15)
            cmds.button(label='load objects', align='center', width=cInnerRowWidth[0] - 30, command=self.reloadButtonFuction)
            cmds.setParent(column1)

            cmds.rowLayout(numberOfColumns=1, columnWidth2=cInnerRowWidth)
            # the UI box containing the list of objects and an array to hold those objects
            self.leftList = cmds.textScrollList(width=cInnerRowWidth[0], ams=True)
            self.leftContents = []
            cmds.setParent(column1)
            
            # UI for all the checkbox filters on the bottom left of the secondary UI
            cmds.rowLayout(numberOfColumns=2, columnWidth2=[(cInnerRowWidth[0]/2), (cInnerRowWidth[0]/2)], ct2 = ['both','both'], co2 = [30,30])
            cmds.columnLayout()
            cmds.checkBox(label='mesh', value=True, cc=partial(self.changeState, 0))
            cmds.checkBox(label='lights', value=True, cc=partial(self.changeState, 1))
            cmds.checkBox(label='cameras', value=True, cc=partial(self.changeState, 2))
            cmds.setParent('..')
            cmds.columnLayout()
            cmds.checkBox(label='groups', value=True, cc=partial(self.changeState, 3))
            cmds.checkBox(label='curves', value=True, cc=partial(self.changeState, 4))
            cmds.checkBox(label='joints', value=True, cc=partial(self.changeState, 5))
            cmds.setParent('..')
            cmds.setParent(column1)
        
            cmds.rowLayout(numberOfColumns=1, columnWidth2=cInnerRowWidth, ct1='both', co1=60)
            cmds.checkBox(label='keyed objects only', value=False, cc=partial(self.changeState, 6))
            cmds.setParent(column1)
        
            cmds.setParent(column1)
        
            # -------------------- in between  column (arrow buttons) --------------------
            
            inter1 = cmds.columnLayout(parent=cPanesLayout, w=45, co=["both", 7])
            # UI for the back and fowards arrows to move items between the two sides/lists
            cmds.button(label = ">>", w = 30, h=30, command=partial(self.addToSelected))
            cmds.separator(h=10)
            cmds.button(label = "<<", w = 30, h=30, command=partial(self.removeFromSelected))
            cmds.setParent(inter1)
        
            # ------------------ panel 2 ------------------
            
            # major differences between 'create' and 'edit' window UI are in this right-hand column
            column2 = cmds.columnLayout(parent = cPanesLayout, w=cAllPanesWidth[0], rowSpacing=10)
            if type == "create":
                cmds.text( parent = column2, label='add items', align='center', w=[cAllPanesWidth[0], cAllPanesWidth[0]][0])
                cmds.rowLayout(numberOfColumns=1, columnWidth2=cInnerRowWidth)
                self.rightList = cmds.textScrollList(width=cInnerRowWidth[0], ams=True)
                self.rightContents = []
            
            else:
                cmds.text( parent = column2, label='edit items', align='center', w=[cAllPanesWidth[0], cAllPanesWidth[0]][0])
                cmds.rowLayout(numberOfColumns=1, columnWidth2=cInnerRowWidth)
                self.rightContents = self.selection.items
                # in edit mode, the right list is initially populated with the items of the selected set
                self.rightList = cmds.textScrollList(width=cInnerRowWidth[0], ams=True, h=280, append=self.rightContents)
        
            cmds.setParent(column2)
            
            # the creation window requires naming funcitonality and UI
            if type == "create":
                cmds.text( label = 'to create a set, enter its name below:', align = 'center', w=[cAllPanesWidth[0], cAllPanesWidth[0]][0])
                cmds.rowLayout(numberOfColumns=2, columnWidth2=[cAllPanesWidth[0]*.25,cAllPanesWidth[0]*.75], ct2=['both', 'both'], co2=[3, 0])
                cmds.text( label = 'name:', align = 'right')
                # UI for text field where the given name will be input by user and can be queried
                self.nameTextField = cmds.textField(ed=True, w=(cAllPanesWidth[0]*.7))
                cmds.setParent(column2) 
            
            cmds.setParent(column2)
    
            # ---------------------- bottom ----------------------
            
            # UI for 'cancel' and 'save' buttons along botom of secondary UI
            cmds.setParent(self.colLayout2)
            cmds.separator()
            cmds.rowLayout(parent=self.colLayout2, numberOfColumns=2, columnWidth2=[300, self.winWidth2*.5], ct2 = ['both','both'])
            cmds.button(label='cancel', height=35, command=partial(self.closeWindow, type))
            if type == "create":
                cmds.button(label='create set', height=35, command=partial(self.createNewSet))
            else:
                cmds.button(label='save set', height=35, command=partial(self.saveSets))
            cmds.setParent(self.colLayout2)
            
            # launches secondary UI window 
            cmds.showWindow(self.winName2)
            cmds.window(self.winName2, e=True, width=self.winWidth2, height=1)      
      
        
    # handles the 'load' function, hierarchies, and filters     
    def reloadButtonFuction(self, *args):
        finalListToShow = []
        curSel = cmds.ls(sl=True)
        
        # ensures that an object is selected when a user presses 'load'
        if len(curSel) == 0:
            cmds.confirmDialog( title='Please select an object in the scene', button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')
        else:
            # allows multiple selection in scene/ouliner
            others = []
            # obtains the decendents for each object selected
            for x in curSel:
                # according the the filters from the checkboxes, only will show the desired object types
                if self.attrArray[0]:
                    temp = cmds.listRelatives(x, ad=True, type='mesh')
                    if temp is not None:
                        for y in temp:
                            others.append(retrieveParent(y))
                if self.attrArray[1]:
                    temp = cmds.listRelatives(x, ad=True, type='light')
                    if temp is not None:
                        for y in temp:
                            others.append(retrieveParent(y))
                if self.attrArray[2]:
                    temp = cmds.listRelatives(x, ad=True, type='camera') 
                    if temp is not None:
                        for y in temp:
                            others.append(retrieveParent(y))
                if self.attrArray[3]:
                    temp = cmds.listRelatives(x, ad=True, typ='transform')
                    if temp is not None:
                        for y in temp:
                            if isAGroup(y):
                                others.append(y)
                if self.attrArray[4]:
                    temp = cmds.listRelatives(x, ad=True, typ="nurbsCurve")
                    if temp is not None:
                        for y in temp:
                            others.append(retrieveParent(y))
                if self.attrArray[5]:
                    temp = cmds.listRelatives(x, ad=True, type='joint')
                    if temp is not None:
                        for y in temp:
                            others.append(y)
                    
            # appends the list of objects that adhere to the filters to the final list
            for o in others:
                finalListToShow.append(o)
            
            # removes duplicates
            self.leftContents = list(set(finalListToShow))
        
            listKeyedObjs = []
            
            # in the filtered list of objects, which have keyframes?
            if self.attrArray[6]:
                if self.leftContents is not None:
                    for z in self.leftContents:
                        if cmds.keyframe(z, kc=True, q=True) > 0:
                            listKeyedObjs.append(z)
                # if 'only keyed objects' is checked, resets the contents to only the keyed ones
                self.leftContents = listKeyedObjs
                
            # refreshes the UI list
            cmds.textScrollList(self.leftList, e=True, ra=True)
            cmds.textScrollList(self.leftList, e=True, append=self.leftContents)
    
    
    # changes the state of the checkbox, and calls the list reload function accordingly
    # reloading refreshes both the object list and the UI
    def changeState(self, *args):
        self.attrArray[args[0]] = not self.attrArray[args[0]]
        self.reloadButtonFuction()
        
        
    # moves items from the left to right lists
    def addToSelected(self, *args):
        listToAdd = []
        # which objects in the panel are selected?
        firstPanelSelected = cmds.textScrollList(self.leftList, q=1, si=1)
        
        # at least one object must be selected, or pop up is given to user
        if firstPanelSelected is None:
            cmds.confirmDialog( title='Please select items to add', button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')
        else:
            for x in firstPanelSelected:
                # ensures the item is not already in the right-hand list
                if not x in set(self.rightContents):
                    listToAdd.append(x)
            
            # refreshes the UI, deslects all items in left-hand list
            cmds.textScrollList(self.rightList, e=True, append=listToAdd)
            cmds.textScrollList(self.leftList, e=True, da=True)
    
    # moves items from the right to left lists    
    def removeFromSelected(self, *args):
        listToRem = []
        # which objects in the panel are selected?
        secondPanelSelected = cmds.textScrollList(self.rightList, q=True, si=True)
        
        # at least one object must be selected, or pop up is given to user
        if secondPanelSelected is None:
            cmds.confirmDialog( title='Please select items to remove', button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')
        else:
            for x in secondPanelSelected:
                # refreshes the UI
                cmds.textScrollList(self.rightList, e=True, ri=x)
                
                
    # destroys the UI window upon pressing the 'cancel' button
    def closeWindow(self, *args):
        self.wasCancelled = True
        cmds.deleteUI(self.winName2)
        
        
    # creates a new set when in the 'create' window
    def createNewSet(self, *args):
        # obtains the new name given by the user via the text field
        nameOfSet = cmds.textField(self.nameTextField, q=True, text=True)
        
        # ensures the new set has been given a unique name
        if (nameOfSet == ""):
            cmds.confirmDialog( title='Please enter a name for your set', button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')
        elif nameOfSet in self.selGrpNames:
            cmds.confirmDialog( title='A set with this name already exists', message="please choose a different name", button=['Okay'], defaultButton='Okay', cancelButton='Okay', dismissString='Okay')
        # when an acceptable name is found, a new Selection group is created
        else:
            # creates a new selection group
            indexOfGroup = len(self.selGrps)
            newGroup = SelGroup(nameOfSet)
            self.selGrps.append(newGroup)
            self.selGrpNames.append(newGroup.name)
            self.selGrps[indexOfGroup].addToGroup(cmds.textScrollList(self.rightList, q=True, allItems=True))
            
            # destroys creation window
            cmds.deleteUI(self.winName2)
            
            self.primaryUI.refreshAfterSecondaryUI(self)
            
        
    # updates and saves the set when in the 'edit' window
    def saveSets(self, *args):
        # index of selection group
        ind = self.selGrpNames.index(self.selection.name)
        
        # reassigns the group's items to be current contents of the right-hand list 
        newItemsList = cmds.textScrollList(self.rightList, q=True, allItems=True)
        
        self.selGrps[ind].items = newItemsList
        self.rightContents = newItemsList
        
        # destroys edit window
        cmds.deleteUI(self.winName2)
        
        # refreshes the primary UI so that the new set name shows up in the sets list
        self.primaryUI.refreshAfterSecondaryUI(self)
    
    
    # returns the list of selection groups and group names after editing by the secondary UI
    def updatedSelGroups(self, *args):
        return self.selGrps
    def updatedSelGroupNames(self, *args):
        return self.selGrpNames
             
# launches the UI          
ui= myUI()