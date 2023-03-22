#include "StateMachine.h"

#include "Logger.h"

template <typename E, typename T0>
bool StateMachine<E, T0>::MoveToState(E State)
{
	if (HandlersMap.Contains(State))
	{
		FStateHandler NextStateHandler = HandlersMap[State];
		if (NextStateHandler.OriginStates.Contains(CurrentState))
		{
			if (HandlersMap.Contains(CurrentState))
			{
				HandlersMap[CurrentState].OnStateEnd.ExecuteIfBound();
			}
			
			CurrentState = State;
			HandlersMap[CurrentState].OnStateBegin.ExecuteIfBound();
			
			return true;
		}
	}
	
	return false;
}

template <typename E, typename T0>
template <class UserClass>
void StateMachine<E, T0>::RegisterStateHandler(E State, const TSet<E>& OriginStates, UserClass* InObject, void (UserClass::*OnBegin)(), void (UserClass::*OnEnd)())
{
	if (OriginStates.Num() == 0)
	{
		Logger::Log(ELogLevel::ERROR, "State machine failed to register new state handler, OriginStates param is not valid.");
		return;
	}
	
	FStateHandler StateHandler(State, OriginStates);

	if (OnBegin)
	{
		StateHandler.OnStateBegin.BindUObject(InObject, OnBegin);
	}
	
	if (OnEnd)
	{
		StateHandler.OnStateEnd.BindUObject(InObject, OnEnd);
	}
	
	HandlersMap.Add(State, StateHandler);
}

template <typename E, typename T0>
void StateMachine<E, T0>::RegisterStateHandler(E State, const TSet<E>& OriginStates)
{
	if (OriginStates.Num() == 0)
	{
		Logger::Log(ELogLevel::ERROR, "State machine failed to register new state handler, OriginStates param is not valid.");
		return;
	}
	
	FStateHandler StateHandler(State, OriginStates);
	HandlersMap.Add(State, StateHandler);
}

template <typename E, typename T0>
void StateMachine<E, T0>::RemoveStateHandler(E State)
{
	HandlersMap.Remove(State);
}

template <typename E, typename T0>
bool StateMachine<E, T0>::CanMoveToState(E State) const
{
	return HandlersMap[CurrentState].OriginStates.Contains(State);
}

