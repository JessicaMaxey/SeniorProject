##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=NanoCom
ConfigurationName      :=Debug
WorkspacePath          := "/home/jess/Documents/Repo/SeniorProject/SeniorProject"
ProjectPath            := "/home/jess/Documents/Repo/SeniorProject/SeniorProject/NanoCom"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=
Date                   :=01/26/16
CodeLitePath           :="/home/jess/.codelite"
LinkerName             :=/opt/cross/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-g++
SharedObjectLinkerName :=/opt/cross/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-g++ -shared -fPIC
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
ObjectsFileList        :="NanoCom.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -lwiringPi
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)./wiringPi/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /opt/cross/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-ar rcu
CXX      := /opt/cross/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-g++
CC       := /opt/cross/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-gcc
CXXFLAGS :=  -g -O0 -std=c++14 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -std=c++14 -std=c++11 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /opt/cross/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/Nano.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "/home/jess/Documents/Repo/SeniorProject/SeniorProject/.build-debug"
	@echo rebuilt > "/home/jess/Documents/Repo/SeniorProject/SeniorProject/.build-debug/NanoCom"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/Nano.cpp$(ObjectSuffix): Nano.cpp $(IntermediateDirectory)/Nano.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jess/Documents/Repo/SeniorProject/SeniorProject/NanoCom/Nano.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Nano.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Nano.cpp$(DependSuffix): Nano.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Nano.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Nano.cpp$(DependSuffix) -MM "Nano.cpp"

$(IntermediateDirectory)/Nano.cpp$(PreprocessSuffix): Nano.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Nano.cpp$(PreprocessSuffix) "Nano.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


