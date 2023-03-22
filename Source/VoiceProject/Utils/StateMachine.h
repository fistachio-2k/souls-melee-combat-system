#pragma once

#include <type_traits>

DECLARE_DELEGATE(FStateHandlerDelegate);

template <typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
class StateMachine
{
	struct FStateHandler
	{
		FStateHandler() = default;
		FStateHandler(E State, const TSet<E>& OriginStates): State(State), OriginStates(OriginStates) {}
		
		E State;
		TSet<E> OriginStates;
		
		FStateHandlerDelegate OnStateBegin;
		FStateHandlerDelegate OnStateEnd;
	};
	
public:
	StateMachine() = default;
	explicit StateMachine(E InitialState) : CurrentState(InitialState)
	{}

	template <class UserClass>
	void RegisterStateHandler(E State, const TSet<E>& OriginStates, UserClass* InObject, void (UserClass::*OnBegin)(), void (UserClass::*OnEnd)() = nullptr);
	void RegisterStateHandler(E State, const TSet<E>& OriginStates);
	void RemoveStateHandler(E State);
	bool MoveToState(E State);
	bool CanMoveToState(E State) const;
	E GetCurrentState() const { return CurrentState; }

private:
	E CurrentState;
	TMap<E, FStateHandler> HandlersMap;
};

#include "StateMachine.cpp"
