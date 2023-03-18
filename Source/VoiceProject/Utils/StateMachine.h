#pragma once

#include <type_traits>

DECLARE_DELEGATE(FStateHandlerDelegate);

template <typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
class StateMachine
{
	struct FStateHandler
	{
		FStateHandler(const E& State): State(State){}

		E State;
		FStateHandlerDelegate OnStateBegin;
		// TODO: add onStateEnd
	};
	
public:
	
	E GetCurrentState() const { return CurrentState; }
	
	void SetState(E State);
	
	template <class UserClass>
	void RegisterStateHandler(E State, UserClass* InObject, void (UserClass::*OnBegin)());

	void RemoveStateHandler(E State);

private:
	E CurrentState;
	TMap<E, FStateHandler> StateHandlers;
};

#include "StateMachine.cpp"
