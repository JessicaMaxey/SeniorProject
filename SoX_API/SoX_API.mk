##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=SoX_API
ConfigurationName      :=Debug
WorkspacePath          := "/home/jess/Documents/Repo/SeniorProject/SeniorProject"
ProjectPath            := "/home/jess/Documents/Repo/SeniorProject/SeniorProject/SoX_API"
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
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="SoX_API.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -lpthread
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
AR       := /opt/cross/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-ar rcu
CXX      := /opt/cross/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-g++
CC       := /opt/cross/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-gcc
CXXFLAGS :=  -g -std=c++14 $(Preprocessors)
CFLAGS   :=  -g $(Preprocessors)
ASFLAGS  := 
AS       := /opt/cross/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/API.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/jess/Documents/Repo/SeniorProject/SeniorProject/.build-debug/SoX_API"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/API.cpp$(ObjectSuffix): API.cpp $(IntermediateDirectory)/API.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jess/Documents/Repo/SeniorProject/SeniorProject/SoX_API/API.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/API.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/API.cpp$(DependSuffix): API.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/API.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/API.cpp$(DependSuffix) -MM "API.cpp"

$(IntermediateDirectory)/API.cpp$(PreprocessSuffix): API.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/API.cpp$(PreprocessSuffix) "API.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


