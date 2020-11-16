// Michal Kowalik <kowalik.michal1@gmail.com>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <memory>

#include <sys/wait.h>
#include <unistd.h>

#include "../common/ip_communication_fifo.h"
#include "log_saver.h"
#include "commands_reader.h"

constexpr char logFileName[] = "log.txt";
constexpr char ImageWriterPath[] = "./ImageWriter/image_writer.out";

pid_t imageWriterPID;

std::unique_ptr<CommandsReader> getCommandsReader(int argc, char* argv[]){
    if (argc > 1){
        return std::unique_ptr<CommandsReader>(new FileCommandsReader("test.in"));
    } else {
        return std::unique_ptr<CommandsReader>(new CINCommandsReader());
    }
}

std::unique_ptr<LogSaver> getLogSaver(){
    return std::unique_ptr<LogSaver>(new FileLogSaver(logFileName));
}

std::unique_ptr<IPCommunication> getIPCommunication(){
    return std::unique_ptr<IPCommunication>(new IPCommunicationFIFO(IPCommunication::Role::CommandsReader));
}

void runImageWriterProc(){
    imageWriterPID = fork(); //< create child process
    if (imageWriterPID == -1){ //< error
        throw std::runtime_error("Error while trying to fork.");
    } else if (imageWriterPID == 0){ //< child process
        execl(ImageWriterPath, "");
        throw std::runtime_error("Error while trying to run ImageWriter.");
    }
    //< return to parent (CommandsReader) process 
}

void waitForImageWriter(){
    int status;
    waitpid(imageWriterPID, &status, 0);
}

int main(int argc, char* argv[]){

    runImageWriterProc();

    auto ipCom = getIPCommunication();
    auto comReader = getCommandsReader(argc, argv);
    auto logSaver = getLogSaver();

    while (true){
        IPCommunication::Request req = comReader->readCommand();
        IPCommunication::Response resp = ipCom->sendRequest(req);
        if (resp.getType() == IPCommunication::Response::ResponseType::ERROR){
            logSaver->log(resp.getMessage(), LogSaver::LogType::ERROR);
        } else if (resp.getType() == IPCommunication::Response::ResponseType::ACK_FINISH){
            break;
        }
    }

    waitForImageWriter();
    return 0;
}