##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=filedialog
ConfigurationName      :=Debug
WorkspacePath          := "/home/jess/Documents/Repo/SeniorProject"
ProjectPath            := "/home/jess/Documents/Repo/SeniorProject/filedialog"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=
Date                   :=03/06/16
CodeLitePath           :="/home/jess/.codelite"
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="filedialog.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  `pkg-config --libs gtk+-3.0 gtkmm-3.0 glibmm-2.4 glib-2.0` 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -std=c++14 -Wall -static-libgcc -static-libstdc++ `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gtkmm-3.0` $(Preprocessors)
CFLAGS   :=  -g -O0 -std=c++14 -Wall -static-libgcc -static-libstdc++ `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gtkmm-3.0` -g -O0 -std=c++14 -Wall -static-libgcc -static-libstdc++ `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gtkmm-3.0` `pkg-config --libs gtk+-3.0` $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/base.cpp$(ObjectSuffix) $(IntermediateDirectory)/examplewindow.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/base.cpp$(ObjectSuffix): base.cpp $(IntermediateDirectory)/base.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jess/Documents/Repo/SeniorProject/filedialog/base.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base.cpp$(DependSuffix): base.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base.cpp$(DependSuffix) -MM "base.cpp"

$(IntermediateDirectory)/base.cpp$(PreprocessSuffix): base.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base.cpp$(PreprocessSuffix) "base.cpp"

$(IntermediateDirectory)/examplewindow.cpp$(ObjectSuffix): examplewindow.cpp $(IntermediateDirectory)/examplewindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jess/Documents/Repo/SeniorProject/filedialog/examplewindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/examplewindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/examplewindow.cpp$(DependSuffix): examplewindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/examplewindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/examplewindow.cpp$(DependSuffix) -MM "examplewindow.cpp"

$(IntermediateDirectory)/examplewindow.cpp$(PreprocessSuffix): examplewindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/examplewindow.cpp$(PreprocessSuffix) "examplewindow.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


