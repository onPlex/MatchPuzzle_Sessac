#include "TileCommandInvoker.h"

// Sets default values
ATileCommandInvoker::ATileCommandInvoker()
{
}

ATileCommandInvoker::~ATileCommandInvoker()
{
	
}

void ATileCommandInvoker::ExecuteCommand(ICommand* Command)
{
	//예시가 단순해서 (X), 캡슐화와 커맨드 패턴을 잘 지향하였다면, 거의 디커플링이 완벽
	Command->Execute();
	CommandHistory.Push(Command);
}

void ATileCommandInvoker::UndoLastCommand()
{
	// 삭제 안할고, '없음' '공백'
	//되돌아가기 -> 되돌아갈 히스토리가 있어야 함
	if(CommandHistory.Num() != 0)
	{
		ICommand* LastCommand = CommandHistory.Last();
		LastCommand->Undo();

		CommandHistory.Pop();
		//CommandHistory.RemoveAll();		
		//CommandHistory.RemoveAt()
	}
}


