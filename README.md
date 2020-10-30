# Branch Prediction
## 如何運行
編譯後使用傳參數的執行方式，傳入參數 - 檔名，之後選定BHT(Bits History Table)的entry大小
### 範例
假如程式編譯為 main.exe，傳入的檔案為test.txt

terminal: `main.exe test.txt`

## 資料格式
### 輸入
instruction的程式，在這裡只做了beq, li, addi
#### 輸入範例

```
	li R1,0
	li R2,4
Loop:
	beq R1,R2,End
	addi R2,R2,-1
	beq R0,R0,Loop
End:
```

### 輸出
顯示出Branch Prediction時的狀態
#### 輸出範例

```
//input: number of entries
8

entry: 2        beq R1,R2,End           //beq R1,R2,End 使用編號2的entry
(00, SN, SN, SN, SN) N N                misprediction: 0
//狀態            預測值 實際值            本預測器miss次數 (從頭統計至今)
entry: 4        beq R0,R0,Loop
(00, SN, SN, SN, SN) N T                misprediction: 1

entry: 2        beq R1,R2,End
(00, SN, SN, SN, SN) N N                misprediction: 0

entry: 4        beq R0,R0,Loop
(01, WN, SN, SN, SN) N T                misprediction: 2

entry: 2        beq R1,R2,End
(00, SN, SN, SN, SN) N N                misprediction: 0

entry: 4        beq R0,R0,Loop
(11, WN, WN, SN, SN) N T                misprediction: 3

entry: 2        beq R1,R2,End
(00, SN, SN, SN, SN) N N                misprediction: 0

entry: 4        beq R0,R0,Loop
(11, WN, WN, SN, WN) N T                misprediction: 4

entry: 2        beq R1,R2,End
(00, SN, SN, SN, SN) N T                misprediction: 1
```
