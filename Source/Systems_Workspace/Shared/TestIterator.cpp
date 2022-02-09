#include "Systems_Workspace/Shared/TestIterators.h"

#define LOG_INT(VAR) UE_LOG(LogTemp, Warning, TEXT("=== " #VAR " = %i"), VAR);
#define LOG_BOOL(VAR) \
	UE_LOG(LogTemp, Warning, TEXT("=== " #VAR " = %s"), VAR ? TEXT("true") : TEXT("false"));
#define LOG_UINT32(VAR) UE_LOG(LogTemp, Warning, TEXT("=== " #VAR " = %u"), VAR);
#define LOG_OPT_INT(VAR)                                                     \
	if (VAR.IsSet())                                                         \
	{                                                                        \
		UE_LOG(LogTemp, Warning, TEXT("=== " #VAR " = %i"), VAR.GetValue()); \
	}                                                                        \
	else                                                                     \
	{                                                                        \
		UE_LOG(LogTemp, Warning, TEXT("=== " #VAR " = <NONE>"));             \
	}
#define LOG_OPT_FLOAT(VAR)                                                   \
	if (VAR.IsSet())                                                         \
	{                                                                        \
		UE_LOG(LogTemp, Warning, TEXT("=== " #VAR " = %f"), VAR.GetValue()); \
	}                                                                        \
	else                                                                     \
	{                                                                        \
		UE_LOG(LogTemp, Warning, TEXT("=== " #VAR " = <NONE>"));             \
	}
#define LOG_INT_ARRAY(VAR) \
	UE_LOG(LogTemp, Warning, TEXT("=== " #VAR " = %s"), *IntArrayToString(VAR));
#define LOG_FLOAT_ARRAY(VAR) \
	UE_LOG(LogTemp, Warning, TEXT("=== " #VAR " = %s"), *FloatArrayToString(VAR));
#define LOG_INT_INT_ARRAY(VAR) \
	UE_LOG(LogTemp, Warning, TEXT("=== " #VAR " = %s"), *IntIntArrayToString(VAR));

FString IntArrayToString(const TArray<int>& Array)
{
	auto Result = FString();
	Result.Append(TEXT("["));
	for (auto Index = 0; Index < Array.Num(); ++Index)
	{
		Result.Append(FString::Printf(TEXT("%i"), Array[Index]));
		if (Index < Array.Num() - 1)
		{
			Result.Append(TEXT(","));
		}
	}
	Result.Append(TEXT("]"));
	return Result;
}

FString FloatArrayToString(const TArray<float>& Array)
{
	auto Result = FString();
	Result.Append(TEXT("["));
	for (auto Index = 0; Index < Array.Num(); ++Index)
	{
		Result.Append(FString::Printf(TEXT("%f"), Array[Index]));
		if (Index < Array.Num() - 1)
		{
			Result.Append(TEXT(","));
		}
	}
	Result.Append(TEXT("]"));
	return Result;
}

FString IntIntArrayToString(const TArray<TTuple<int, int>>& Array)
{
	auto Result = FString();
	Result.Append(TEXT("["));
	for (auto Index = 0; Index < Array.Num(); ++Index)
	{
		Result.Append(TEXT("("));
		Result.Append(FString::Printf(TEXT("%i"), Array[Index].Get<0>()));
		Result.Append(TEXT(","));
		Result.Append(FString::Printf(TEXT("%i"), Array[Index].Get<1>()));
		Result.Append(TEXT(")"));
		if (Index < Array.Num() - 1)
		{
			Result.Append(TEXT(","));
		}
	}
	Result.Append(TEXT("]"));
	return Result;
}

void TestIterators()
{
	// [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	const TArray<int> A = IterRange(0, 10).CollectArray();
	LOG_INT_ARRAY(A);

	// 10
	const uint32 B = IterRange(0, 10).Count();
	LOG_UINT32(B);

	// 5
	const TOptional<int> C = IterRange(0, 10).Nth(5);
	LOG_OPT_INT(C);

	// 45
	const int D = IterRange(0, 10).Fold(
		0, [](const auto& Accum, const auto& Value) { return Accum + Value; });
	LOG_INT(D);

	// 5
	const TOptional<int> E = IterRange(0, 10).Find([](const auto& Value) { return Value >= 5; });
	LOG_OPT_INT(E);

	// 5.0
	const TOptional<float> F = IterRange(0, 10).FindMap<float>(
		[](const auto& Value)
		{
			if (Value >= 5)
			{
				return TOptional<float>(static_cast<float>(Value));
			}
			else
			{
				return TOptional<float>();
			}
		});
	LOG_OPT_FLOAT(F);

	// [0, 2, 4, 6, 8, 10, 12, 14, 16, 18]
	const TArray<int> G =
		IterRange(0, 10).Map<int>([](const auto& Value) { return Value * 2; }).CollectArray();
	LOG_INT_ARRAY(G);

	// [0, 2, 4, 6, 8]
	const TArray<int> H =
		IterRange(0, 10).Filter([](const auto& Value) { return Value % 2 == 0; }).CollectArray();
	LOG_INT_ARRAY(H);

	// [0.0, 2.0, 4.0, 6.0, 8.0]
	const TArray<float> I = IterRange(0, 10)
								.FilterMap<float>(
									[](const auto& Value)
									{
										if (Value % 2 == 0)
										{
											return TOptional<float>(static_cast<float>(Value));
										}
										else
										{
											return TOptional<float>();
										}
									})
								.CollectArray();
	LOG_FLOAT_ARRAY(I);

	// [(0, -5), (1, -4), (2, -3), (3, -2), (4, -1)]
	const TArray<TTuple<int, int>> J = IterRange(0, 5).Zip(IterRange(-5, 0)).CollectArray();
	LOG_INT_INT_ARRAY(J);

	// [0, 1, 2, 3, 4, -5, -4, -3, -2, -1]
	const TArray<int> K = IterRange(0, 5).Chain(IterRange(-5, 0)).CollectArray();
	LOG_INT_ARRAY(K);

	// [0]
	const TArray<int> L = IterOnce(0).CollectArray();
	LOG_INT_ARRAY(L);

	// [1, 1, 1, 1, 1]
	const TArray<int> M = IterRepeat(1).Take(5).CollectArray();
	LOG_INT_ARRAY(M);

	// [3, 4, 5, 6, 7]
	const TArray<int> N = IterRange(0, 10).Skip(3).Take(5).CollectArray();
	LOG_INT_ARRAY(N);

	// [?, ?, ?, ?, ?]
	const TArray<int> O = IterGenerate<int>([]() { return FMath::Rand(); }).Take(5).CollectArray();
	LOG_INT_ARRAY(O);

	// [0, 1, 2, 3, 4, 0, 1, 2, 3, 4]
	const TArray<int> P = IterGenerate<TIterRange<int>>([]() { return IterRange(0, 5); })
							  .Take(2)
							  .Flatten<int>()
							  .CollectArray();
	LOG_INT_ARRAY(P);

	// true
	const bool Q =
		IterStdConst(A)
			.Enumerate()
			.Map<bool>([](const auto& Pair) { return Pair.Get<0>() == Pair.Get<1>(); })
			.Fold(true, [](const auto Accum, const auto Value) { return Accum && Value; });
	LOG_BOOL(Q);

	// [0, 4, 16, 36, 64]
	const TArray<int> R = IterStdConst(A)
							  .Filter([](const auto& Value) { return Value % 2 == 0; })
							  .Map<int>([](const auto& Value) { return Value * Value; })
							  .CollectArray();
	LOG_INT_ARRAY(R);

	// [1, 3, 5, 7, 9]
	const TArray<int> S = IterRange(0, 10).Adapt(TIterOddAdapter<int>()).CollectArray();
	LOG_INT_ARRAY(S);

	// [(0, 1), (1, 2), (2, 3), (3, 4)]
	const TArray<TTuple<int, int>> T =
		IterRange(0, 5)
			.Views<2>()
			.Map<TTuple<int, int>>([](const auto& View) { return MakeTuple(View[0], View[1]); })
			.CollectArray();
	LOG_INT_INT_ARRAY(T);

	// [0.0, 1.0, 2.0, 3.0, 4.0]
	const TArray<float> U = IterRange(0, 5).Cast<float>().CollectArray();
	LOG_FLOAT_ARRAY(U);
}
