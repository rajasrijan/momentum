#
#  Copyright 2009-2021 Srijan Kumar Sharma
#
#  This file is part of Momentum.
#
#  Momentum is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Momentum is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Momentum.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.16)

find_package(Git)
if(NOT Git_FOUND)
  message(ERROR "Git not found")
endif()

execute_process(
    COMMAND ${GIT_EXECUTABLE} log --oneline -1
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/acpica
    OUTPUT_VARIABLE GIT_LOG
)

if(NOT (GIT_LOG MATCHES "Add momentum OS"))
  execute_process(
    COMMAND ${GIT_EXECUTABLE} am ${CMAKE_CURRENT_SOURCE_DIR}/0001-Add-momentum-OS.patch
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/acpica
    OUTPUT_VARIABLE GIT_LOG
    RESULT_VARIABLE PATCH_APPLY_RESULT
  )
  if(NOT (${PATCH_APPLY_RESULT} equals 0))
    message(ERROR "Failed to apply git patch: ${GIT_LOG}")
  endif()
endif()

set(ACPICA_SRC 
# acpica/source/tools/efihello/efihello.c
# acpica/source/tools/acpihelp/ahaml.c
# acpica/source/tools/acpihelp/ahaslops.c
# acpica/source/tools/acpihelp/ahgrammar.c
# acpica/source/tools/acpihelp/ahamlops.c
# acpica/source/tools/acpihelp/ahaslkey.c
# acpica/source/tools/acpihelp/ahdecode.c
# acpica/source/tools/acpihelp/ahmain.c
# acpica/source/tools/acpihelp/ahasl.c
# acpica/source/tools/examples/examples.c
# acpica/source/tools/examples/extables.c
# acpica/source/tools/examples/exstubs.c
# acpica/source/tools/acpidump/apmain.c
# acpica/source/tools/acpidump/apfiles.c
# acpica/source/tools/acpidump/apdump.c
# acpica/source/tools/acpixtract/axmain.c
# acpica/source/tools/acpixtract/axutils.c
# acpica/source/tools/acpixtract/acpixtract.c
# acpica/source/tools/acpibin/abmain.c
# acpica/source/tools/acpibin/abcompare.c
# acpica/source/tools/acpisrc/asremove.c
# acpica/source/tools/acpisrc/asutils.c
# acpica/source/tools/acpisrc/asmain.c
# acpica/source/tools/acpisrc/asconvrt.c
# acpica/source/tools/acpisrc/ascase.c
# acpica/source/tools/acpisrc/asfile.c
# acpica/source/tools/acpisrc/astable.c
# acpica/source/tools/acpiexec/aemain.c
# acpica/source/tools/acpiexec/aeinitfile.c
# acpica/source/tools/acpiexec/aeregion.c
# acpica/source/tools/acpiexec/aehandlers.c
# acpica/source/tools/acpiexec/aeinstall.c
# acpica/source/tools/acpiexec/aeexception.c
# acpica/source/tools/acpiexec/aetables.c
# acpica/source/tools/acpiexec/aeexec.c
# acpica/source/tools/acpiexec/aetests.c
# acpica/source/components/debugger/dbexec.c
# acpica/source/components/debugger/dbcmds.c
# acpica/source/components/debugger/dbnames.c
# acpica/source/components/debugger/dbinput.c
# acpica/source/components/debugger/dbxface.c
# acpica/source/components/debugger/dbfileio.c
# acpica/source/components/debugger/dbobject.c
# acpica/source/components/debugger/dbconvert.c
# acpica/source/components/debugger/dbtest.c
# acpica/source/components/debugger/dbdisply.c
# acpica/source/components/debugger/dbhistry.c
# acpica/source/components/debugger/dbstats.c
# acpica/source/components/debugger/dbmethod.c
# acpica/source/components/debugger/dbutils.c
acpica/source/components/tables/tbxfload.c
acpica/source/components/tables/tbxface.c
acpica/source/components/tables/tbxfroot.c
acpica/source/components/tables/tbinstal.c
acpica/source/components/tables/tbfind.c
acpica/source/components/tables/tbutils.c
acpica/source/components/tables/tbprint.c
acpica/source/components/tables/tbdata.c
acpica/source/components/tables/tbfadt.c
acpica/source/components/resources/rsinfo.c
acpica/source/components/resources/rscreate.c
acpica/source/components/resources/rsserial.c
acpica/source/components/resources/rsio.c
acpica/source/components/resources/rscalc.c
acpica/source/components/resources/rsxface.c
acpica/source/components/resources/rsutils.c
acpica/source/components/resources/rsmemory.c
acpica/source/components/resources/rsaddr.c
acpica/source/components/resources/rsmisc.c
acpica/source/components/resources/rslist.c
acpica/source/components/resources/rsirq.c
acpica/source/components/resources/rsdump.c
acpica/source/components/resources/rsdumpinfo.c
acpica/source/components/dispatcher/dsutils.c
acpica/source/components/dispatcher/dsinit.c
acpica/source/components/dispatcher/dswstate.c
acpica/source/components/dispatcher/dsmethod.c
acpica/source/components/dispatcher/dswscope.c
acpica/source/components/dispatcher/dspkginit.c
acpica/source/components/dispatcher/dsargs.c
acpica/source/components/dispatcher/dsobject.c
acpica/source/components/dispatcher/dsdebug.c
acpica/source/components/dispatcher/dsopcode.c
acpica/source/components/dispatcher/dswload.c
acpica/source/components/dispatcher/dsmthdat.c
acpica/source/components/dispatcher/dswload2.c
acpica/source/components/dispatcher/dswexec.c
acpica/source/components/dispatcher/dscontrol.c
acpica/source/components/dispatcher/dsfield.c
acpica/source/components/parser/psargs.c
acpica/source/components/parser/pswalk.c
acpica/source/components/parser/psscope.c
acpica/source/components/parser/pstree.c
acpica/source/components/parser/psobject.c
acpica/source/components/parser/psloop.c
acpica/source/components/parser/psxface.c
acpica/source/components/parser/psutils.c
acpica/source/components/parser/psopcode.c
acpica/source/components/parser/psopinfo.c
acpica/source/components/parser/psparse.c
acpica/source/components/events/evsci.c
acpica/source/components/events/evgpeutil.c
acpica/source/components/events/evhandler.c
acpica/source/components/events/evxfregn.c
acpica/source/components/events/evxfgpe.c
acpica/source/components/events/evgpeinit.c
acpica/source/components/events/evglock.c
acpica/source/components/events/evregion.c
acpica/source/components/events/evevent.c
acpica/source/components/events/evxface.c
acpica/source/components/events/evxfevnt.c
acpica/source/components/events/evmisc.c
acpica/source/components/events/evgpeblk.c
acpica/source/components/events/evrgnini.c
acpica/source/components/events/evgpe.c
acpica/source/components/namespace/nsutils.c
acpica/source/components/namespace/nsdump.c
acpica/source/components/namespace/nsalloc.c
acpica/source/components/namespace/nsrepair.c
acpica/source/components/namespace/nsprepkg.c
acpica/source/components/namespace/nsinit.c
acpica/source/components/namespace/nsnames.c
acpica/source/components/namespace/nsrepair2.c
acpica/source/components/namespace/nseval.c
acpica/source/components/namespace/nspredef.c
acpica/source/components/namespace/nsload.c
acpica/source/components/namespace/nsxfname.c
acpica/source/components/namespace/nsconvert.c
acpica/source/components/namespace/nsobject.c
acpica/source/components/namespace/nsdumpdv.c
acpica/source/components/namespace/nssearch.c
acpica/source/components/namespace/nswalk.c
acpica/source/components/namespace/nsaccess.c
acpica/source/components/namespace/nsarguments.c
acpica/source/components/namespace/nsparse.c
acpica/source/components/namespace/nsxfobj.c
acpica/source/components/namespace/nsxfeval.c
acpica/source/components/utilities/utownerid.c
acpica/source/components/utilities/utosi.c
acpica/source/components/utilities/utstring.c
acpica/source/components/utilities/utxferror.c
acpica/source/components/utilities/utresrc.c
acpica/source/components/utilities/utalloc.c
acpica/source/components/utilities/utdecode.c
acpica/source/components/utilities/uterror.c
acpica/source/components/utilities/utmath.c
acpica/source/components/utilities/utxfmutex.c
acpica/source/components/utilities/utresdecode.c
acpica/source/components/utilities/uteval.c
acpica/source/components/utilities/utbuffer.c
acpica/source/components/utilities/uttrack.c
acpica/source/components/utilities/utobject.c
acpica/source/components/utilities/utdelete.c
acpica/source/components/utilities/utuuid.c
acpica/source/components/utilities/utids.c
acpica/source/components/utilities/utstrsuppt.c
acpica/source/components/utilities/utstate.c
acpica/source/components/utilities/utxfinit.c
acpica/source/components/utilities/uthex.c
acpica/source/components/utilities/utstrtoul64.c
acpica/source/components/utilities/utnonansi.c
acpica/source/components/utilities/utdebug.c
acpica/source/components/utilities/utmutex.c
acpica/source/components/utilities/utclib.c
acpica/source/components/utilities/utascii.c
acpica/source/components/utilities/utinit.c
acpica/source/components/utilities/utpredef.c
acpica/source/components/utilities/utprint.c
acpica/source/components/utilities/utglobal.c
acpica/source/components/utilities/utaddress.c
acpica/source/components/utilities/utxface.c
acpica/source/components/utilities/utlock.c
acpica/source/components/utilities/utcache.c
acpica/source/components/utilities/utcopy.c
acpica/source/components/utilities/utexcep.c
acpica/source/components/utilities/utmisc.c
# acpica/source/components/disassembler/dmutils.c
# acpica/source/components/disassembler/dmresrcs.c
# acpica/source/components/disassembler/dmopcode.c
# acpica/source/components/disassembler/dmcstyle.c
# acpica/source/components/disassembler/dmresrcl2.c
# acpica/source/components/disassembler/dmresrc.c
# acpica/source/components/disassembler/dmbuffer.c
# acpica/source/components/disassembler/dmwalk.c
# acpica/source/components/disassembler/dmdeferred.c
# acpica/source/components/disassembler/dmresrcl.c
# acpica/source/components/disassembler/dmnames.c
acpica/source/components/executer/exnames.c
acpica/source/components/executer/exstoren.c
acpica/source/components/executer/exoparg3.c
acpica/source/components/executer/exresnte.c
acpica/source/components/executer/exprep.c
acpica/source/components/executer/exconvrt.c
acpica/source/components/executer/exmisc.c
acpica/source/components/executer/exstorob.c
acpica/source/components/executer/exconcat.c
acpica/source/components/executer/exresolv.c
acpica/source/components/executer/exregion.c
acpica/source/components/executer/exutils.c
acpica/source/components/executer/exfldio.c
acpica/source/components/executer/exoparg6.c
acpica/source/components/executer/exresop.c
acpica/source/components/executer/exdump.c
acpica/source/components/executer/exmutex.c
acpica/source/components/executer/exconfig.c
acpica/source/components/executer/exoparg2.c
acpica/source/components/executer/exsystem.c
acpica/source/components/executer/exfield.c
acpica/source/components/executer/exstore.c
acpica/source/components/executer/exdebug.c
acpica/source/components/executer/exoparg1.c
acpica/source/components/executer/exserial.c
acpica/source/components/executer/excreate.c
acpica/source/components/executer/extrace.c
acpica/source/components/hardware/hwpci.c
acpica/source/components/hardware/hwregs.c
acpica/source/components/hardware/hwxfsleep.c
acpica/source/components/hardware/hwgpe.c
acpica/source/components/hardware/hwxface.c
acpica/source/components/hardware/hwtimer.c
acpica/source/components/hardware/hwacpi.c
acpica/source/components/hardware/hwvalid.c
acpica/source/components/hardware/hwsleep.c
acpica/source/components/hardware/hwesleep.c
# acpica/source/common/ahpredef.c
# acpica/source/common/dmtable.c
# acpica/source/common/dmtbinfo3.c
# acpica/source/common/dmtbinfo1.c
# acpica/source/common/dmtbdump.c
# acpica/source/common/ahids.c
# acpica/source/common/dmtbdump1.c
# acpica/source/common/dmtbinfo2.c
# acpica/source/common/dmextern.c
# acpica/source/common/dmtbdump2.c
# acpica/source/common/acgetline.c
# acpica/source/common/adfile.c
# acpica/source/common/dmtbinfo.c
# acpica/source/common/adisasm.c
# acpica/source/common/getopt.c
# acpica/source/common/dmtables.c
# acpica/source/common/dmrestag.c
# acpica/source/common/dmtbdump3.c
# acpica/source/common/dmswitch.c
# acpica/source/common/adwalk.c
# acpica/source/common/acfileio.c
# acpica/source/common/ahuuids.c
# acpica/source/common/ahtable.c
# acpica/source/common/cmfsize.c
# acpica/source/compiler/aslhelp.c
# acpica/source/compiler/aslmap.c
# acpica/source/compiler/aslrestype2w.c
# acpica/source/compiler/dtsubtable.c
# acpica/source/compiler/dttable2.c
# acpica/source/compiler/asloptions.c
# acpica/source/compiler/aslmain.c
# acpica/source/compiler/aslutils.c
# acpica/source/compiler/aslxrefout.c
# acpica/source/compiler/aslbtypes.c
# acpica/source/compiler/asloffset.c
# acpica/source/compiler/aslmapoutput.c
# acpica/source/compiler/aslprepkg.c
# acpica/source/compiler/aslcodegen.c
# acpica/source/compiler/aslexternal.c
# acpica/source/compiler/aslpredef.c
# acpica/source/compiler/aslmethod.c
# acpica/source/compiler/aslrestype1.c
# acpica/source/compiler/aslcache.c
# acpica/source/compiler/aslmessages.c
# acpica/source/compiler/prmacros.c
# acpica/source/compiler/dtutils.c
# acpica/source/compiler/aslrestype2s.c
# acpica/source/compiler/aslrestype2d.c
# acpica/source/compiler/dtexpress.c
# acpica/source/compiler/asldebug.c
# acpica/source/compiler/dttable.c
# acpica/source/compiler/aslprintf.c
# acpica/source/compiler/asllistsup.c
# acpica/source/compiler/prexpress.c
# acpica/source/compiler/aslload.c
# acpica/source/compiler/asllength.c
# acpica/source/compiler/aslcompile.c
# acpica/source/compiler/asllisting.c
# acpica/source/compiler/dtio.c
# acpica/source/compiler/aslparseop.c
# acpica/source/compiler/cvparser.c
# acpica/source/compiler/asloperands.c
# acpica/source/compiler/aslopcodes.c
# acpica/source/compiler/aslpld.c
# acpica/source/compiler/aslnamesp.c
# acpica/source/compiler/aslfold.c
# acpica/source/compiler/aslhex.c
# acpica/source/compiler/aslfileio.c
# acpica/source/compiler/asltransform.c
# acpica/source/compiler/aslanalyze.c
# acpica/source/compiler/aslallocate.c
# acpica/source/compiler/aslfiles.c
# acpica/source/compiler/aslopt.c
# acpica/source/compiler/aslrestype2e.c
# acpica/source/compiler/aslmaputils.c
# acpica/source/compiler/dttemplate.c
# acpica/source/compiler/aslrestype2q.c
# acpica/source/compiler/aslxref.c
# acpica/source/compiler/asluuid.c
# acpica/source/compiler/dtfield.c
# acpica/source/compiler/aslrestype2.c
# acpica/source/compiler/cvcompiler.c
# acpica/source/compiler/prscan.c
# acpica/source/compiler/aslmapenter.c
# acpica/source/compiler/aslstartup.c
# acpica/source/compiler/asllookup.c
# acpica/source/compiler/aslprune.c
# acpica/source/compiler/prutils.c
# acpica/source/compiler/asltree.c
# acpica/source/compiler/aslstubs.c
# acpica/source/compiler/aslerror.c
# acpica/source/compiler/dtcompile.c
# acpica/source/compiler/cvdisasm.c
# acpica/source/compiler/aslresource.c
# acpica/source/compiler/aslrestype1i.c
# acpica/source/compiler/dttable1.c
# acpica/source/compiler/aslascii.c
# acpica/source/compiler/aslwalks.c
# acpica/source/os_specific/efi/oseficlib.c
# acpica/source/os_specific/efi/osefixf.c
# acpica/source/os_specific/efi/osefitbl.c
# acpica/source/os_specific/service_layers/osunixmap.c
# acpica/source/os_specific/service_layers/oswindir.c
# acpica/source/os_specific/service_layers/oswinxf.c
# acpica/source/os_specific/service_layers/osbsdtbl.c
# acpica/source/os_specific/service_layers/osunixxf.c
# acpica/source/os_specific/service_layers/osgendbg.c
# acpica/source/os_specific/service_layers/oswintbl.c
# acpica/source/os_specific/service_layers/osunixdir.c
# acpica/source/os_specific/service_layers/oslinuxtbl.c
)

add_library(acpica STATIC ${ACPICA_SRC})
target_include_directories(acpica PUBLIC acpica/source/include)
target_include_directories(acpica PUBLIC ../libc/include)

target_link_libraries(momentum acpica)          #   add self to main project