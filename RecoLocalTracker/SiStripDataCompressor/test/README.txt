One way to add it is with LDFLAGS in the BuildFile.xml of your CMSSW package
<flags LDFLAGS="-lLibName -LLibPath"/>

The more appropriate way is to place the library in the external package
known to scram.

For private tests the external can be set up using a tool.xml file, e.g.
taking randomly from evtgen
https://github.com/cms-sw/cmsdist/blob/IB/CMSSW_11_3_X/master/evtgen-toolfile.spec

<tool name="MyTool" version="version">
 <lib name="MyTool"/>
 <client>
   <environment name="MyToolBASE" default="PATH_TO_THE_DIR"/>
   <environment name="LIBDIR" default="$MyToolBASE/lib"/>
   <environment name="INCLUDE" default="$MyToolBASE/include"/>
 </client>
 <runtime name="MyTool_BASE" value="$MyTool_BASE"/>
 <use name="hepmc"/>
</tool>

after that you do "scram setup tool.xml "
and then in the CMSSW package that depends on it you would have
in the BuildFile.xml
<use name="MyTool"/>