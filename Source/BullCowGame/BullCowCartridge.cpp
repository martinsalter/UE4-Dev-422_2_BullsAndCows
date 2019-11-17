// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);
    SetupGame();

    PrintLine(FString::Printf(TEXT("The Hidden word is %s"), *HiddenWord)); // DEBUG line
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome to BullCows!"));

    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num()-1)];
    Lives = HiddenWord.Len();
    bGameOver = false;
    
    PrintLine(FString::Printf(TEXT("Guess the %i-letter word"), HiddenWord.Len()));
    PrintLine(FString::Printf(TEXT("You have %i Lives"), Lives));
    PrintLine(TEXT("Enter your guess..."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press Enter to play again."));
}

void UBullCowCartridge::TryAgain(const FString& Message) const
{
    ClearScreen();
    PrintLine(Message);
    PrintLine(FString::Printf(TEXT("Remember, %i letters. Isogram."),HiddenWord.Len()));
    PrintLine(TEXT("Try again..."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(Guess == HiddenWord)
    {
        PrintLine(Guess + TEXT(" is correct. Well Done!"));
        EndGame();
        return;
    }
    
    if(HiddenWord.Len() != Guess.Len())
    {
        TryAgain(FString::Printf(TEXT("%i characters, I said!"), HiddenWord.Len()));
        return;
    }

    if (!IsIsogram(Guess)) 
    {
        TryAgain(TEXT("That's not even an isogram, fool!"));
        return;
    }

    if (--Lives > 0)
    {
        FBullCowCount Count = GetBullsCows(Guess);
        TryAgain(FString::Printf(TEXT("%s? Seriously!?\n%i Bulls / %i Cows\n%i lives remaining"), *Guess, Count.Bulls, Count.Cows, Lives));
        return;
    }
    ClearScreen();
    PrintLine(TEXT("FAIL! Fucking Loser."));
    PrintLine(FString::Printf(TEXT("The word was %s.\nObviously."),*HiddenWord));
    EndGame();
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 i = 0; i < Word.Len() -1; i++)
    {
        for (int32 j = i+1; j < Word.Len(); j++)
        {
            if (Word[j] == Word[i]) 
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& FullWordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : FullWordList)
    {
        if (Word.Len() >=4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullsCows(const FString& Guess) const
{
    FBullCowCount Count;
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if(Guess[GuessIndex] == HiddenWord[GuessIndex]) 
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < Guess.Len(); HiddenIndex++)
        {
            if (HiddenIndex == GuessIndex) continue;
            if (Guess[HiddenIndex] == Guess[GuessIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}