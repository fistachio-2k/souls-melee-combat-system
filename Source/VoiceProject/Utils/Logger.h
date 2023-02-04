#pragma once

UENUM(BlueprintType)
enum class ELogLevel: uint8 {
	TRACE			UMETA(DisplayName = "Trace"),
	DEBUG			UMETA(DisplayName = "Debug"),
	INFO			UMETA(DisplayName = "Info"),
	WARNING			UMETA(DisplayName = "Warning"),
	ERROR			UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class ELogOutput: uint8 {
	ALL				UMETA(DisplayName = "All levels"),
	OUTPUT_LOG		UMETA(DisplayName = "Output log"),
	SCREEN			UMETA(DisplayName = "Screen")
};

class Logger
{
public:
	/**
	* Log - prints a message to all the log outputs with a specific color
	* @param LogLevel {@see ELogLevel} affects color of log
	* @param FString the message for display
	*/
	static void Log(ELogLevel LogLevel, FString Message);
	/**
	* Log - prints a message to all the log outputs with a specific color
	* @param Level {@see ELogLevel} affects color of log
	* @param Message the message for display
	* @param LogOutput - All, Output Log or Screen
	*/
	static void Log(ELogLevel Level, FString Message, ELogOutput LogOutput);
};
