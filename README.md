# EBMS_BANKING
# Build the application
    make clean 
    make all

    Note: will add debug target in future to enable the debugging in gdb  by setting -g dynamically

for every module (if you want to build any separate dynamic/shared library) create a make and include in main make file. Ex:

    CXX = g++
    CXXFLAGS = -std=c++20 -Wall -g -Wextra -Iinclude -fPIC -I../common/include -I../Logger/include -I../utils/include

    SRC = $(wildcard src/*.cpp)
    OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))

    TARGET = ../lib/libCommon.so

    all: create_dirs $(TARGET)

    create_dirs:
        mkdir -p build
        mkdir -p ../lib

    $(TARGET): $(OBJ)
        $(CXX) -shared -o $@ $(OBJ)

    build/%.o: src/%.cpp
        $(CXX) $(CXXFLAGS) -c $< -o $@

    clean:
	rm -rf build
	rm -f $(TARGET)

# Runnig the Application

    ./scripts/run.sh
    
    or

    .bin/BankingSystem

## ORACLE Docker container installation
1. pull the docker image by using below command.
    docker pull container-registry.oracle.com/database/free:latest
2. Create a persistent memory mount for oracle data
    docker volume create oracledata
3. Run the docker container with latest image

    docker run -d \
  --name oracle-free \
  -p 1521:1521 \
  -p 5500:5500 \
  -e ORACLE_PWD=Oracle@123 \
  -v oracledata:/opt/oracle/oradata \
  container-registry.oracle.com/database/free:latest

4. connect to the bash shell of the docker container.
    docker exec -it oracle-free bash
5. Verify the db by connecting to DB using below command.
   
        bash-4.4$ sqlplus / as sysdba

6. verify the listener status by connecting to container machine using below command.

        lsnrctl status

7. or directly verify from machine using docker command.
    docker exec -it oracle-free lsnrctl status

    Connecting to (DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=0.0.0.0)(PORT=1521)))
STATUS of the LISTENER
------------------------
Alias                     LISTENER
Version                   TNSLSNR for Linux: Version 23.26.2.0.0 - Production
Start Date                03-AUG-2026 11:44:28
Uptime                    0 days 0 hr. 9 min. 13 sec
Trace Level               off
Security                  ON: Local OS Authentication
SNMP                      OFF
Default Service           FREE
Listener Parameter File   /opt/oracle/product/26ai/dbhomeFree/network/admin/listener.ora
Listener Log File         /opt/oracle/diag/tnslsnr/3bc5d11c4413/listener/alert/log.xml
Listening Endpoints Summary...
  (DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST=0.0.0.0)(PORT=1521)))
  (DESCRIPTION=(ADDRESS=(PROTOCOL=ipc)(KEY=EXTPROC1521)))
Services Summary...
Service "50905d4e836e0814e0630800580af1bc" has 1 instance(s).
  Instance "FREE", status READY, has 1 handler(s) for this service...
Service "FREE" has 1 instance(s).
  Instance "FREE", status READY, has 1 handler(s) for this service...
Service "FREEXDB" has 1 instance(s).
  Instance "FREE", status READY, has 1 handler(s) for this service...
Service "freepdb1" has 1 instance(s).
  Instance "FREE", status READY, has 1 handler(s) for this service...
The command completed successfully