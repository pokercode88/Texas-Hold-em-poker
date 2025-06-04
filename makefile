
#-----------------------------------------------------------------------

APP           := XGame
TARGET        := GMServer
CONFIG        := 
STRIP_FLAG    := N
TARS2CPP_FLAG := 
CFLAGS        += -lm -Wunused-but-set-variable
CXXFLAGS      += -lm -Wunused-but-set-variable

INCLUDE   += -I/usr/local/cpp_modules/wbl/include
LIB       += -L/usr/local/cpp_modules/wbl/lib -lwbl

INCLUDE   += -I/usr/local/cpp_modules/rapidjson/include
LIB       += -L/usr/local/cpp_modules/rapidjson/

INCLUDE   += -I/usr/local/cpp_modules/protobuf/include
LIB       += -L/usr/local/cpp_modules/protobuf/lib -lprotobuf

INCLUDE   += -I/usr/local/cpp_modules/maxminddb/include
LIB       += -L/usr/local/cpp_modules/maxminddb/lib -lmaxminddb

INCLUDE   += -I/usr/local/mysql/include
LIB       += -L/usr/local/mysql/lib/mysql -lmysqlclient

#-----------------------------------------------------------------------
include /home/tarsproto/XGame/Comm/Comm.mk
include /home/tarsproto/XGame/util/util.mk
include /home/tarsproto/XGame/ConfigServer/ConfigServer.mk
include /home/tarsproto/XGame/HallServer/HallServer.mk
include /home/tarsproto/XGame/GlobalServer/GlobalServer.mk
include /home/tarsproto/XGame/PushServer/PushServer.mk
include /home/tarsproto/XGame/OrderServer/OrderServer.mk
include /home/tarsproto/XGame/SocialServer/SocialServer.mk
include /home/tarsproto/XGame/protocols/protocols.mk
include /usr/local/tars/cpp/makefile/makefile.tars
include /home/tarsproto/XGame/RoomServer/RoomServer.mk

#-----------------------------------------------------------------------

xgame:
	cp -f $(TARGET) /usr/local/app/tars/tarsnode/data/XCommon.GMServer/bin/

100:
	sshpass -p 'awzs2022' scp ./GMServer root@10.10.10.100:/home/yuj/server/gmserver
