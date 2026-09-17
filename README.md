# 資料結構作業一：動態環狀佇列

學號：114703059　姓名：李秉澄

執行環境：

`Windows 11 (x86_64), g++ (MinGW-W64 x86_64-ucrt-posix-seh) 14.2.0 (-std=c++20), python 3.10.11`

本作業不指定開發環境。下列指令即為評分時使用的指令，沒有隱藏條件；請自行確認你的程式全部通過。

## 一、檔案分工

- 需要完成：`circular_queue.cpp`、`tests.cpp`、`answer_sheet.md`、`ai_disclosure.md`、`README.md`（補上執行環境）。
- 教師提供且不得修改：`naive_shift_queue.hpp`、`public_tests.cpp`、`benchmark.cpp`、`plot_results.py`。
- `circular_queue.hpp` 的公開介面不得修改；可使用已提供的私有 `grow()` 輔助函式。

## 二、編譯並執行公開測試

```bash
g++ -std=c++20 -O2 -Wall -Wextra -pedantic \
    public_tests.cpp circular_queue.cpp -o public_tests
./public_tests
```

應輸出 `All public tests passed.`，且編譯過程不應出現任何警告。

## 三、編譯並執行自己設計的測試

```bash
g++ -std=c++20 -O2 -Wall -Wextra -pedantic \
    tests.cpp circular_queue.cpp -o tests
./tests
```

請在 `tests.cpp` 中以具名常數記錄隨機差異測試所使用的亂數種子。

## 四、檢查記憶體安全

記憶體安全會以 AddressSanitizer 與 UndefinedBehaviorSanitizer 評分。請自行執行下列檢查：

```bash
g++ -std=c++20 -g -Wall -Wextra -pedantic \
    -fsanitize=address,undefined -fno-omit-frame-pointer \
    tests.cpp circular_queue.cpp -o tests_asan
./tests_asan
```

執行期間不應出現記憶體洩漏、陣列越界、重複釋放、釋放後繼續使用或其他未定義行為的回報。

在 Linux 上 LeakSanitizer 預設為開啟，上述指令即可偵測記憶體洩漏，不需要額外加參數。macOS 不支援 LeakSanitizer，加參數也無效：若你在 macOS 開發，繳交前請務必找一台 Linux 機器或容器重跑一次這道檢查，否則忘記在解構函式釋放記憶體時，你在本機會看不到任何錯誤訊息。

## 五、執行效能實驗

```bash
g++ -std=c++20 -O2 -Wall -Wextra -pedantic \
    benchmark.cpp circular_queue.cpp -o benchmark
./benchmark > results.csv
python3 plot_results.py results.csv performance.png
```

`plot_results.py` 需要 Python 3 與 Matplotlib（可用 `pip install matplotlib` 安裝）。它會產生 `performance.png`，並在終端機列出兩種實作於各資料規模下的中位執行時間；請將這些數值填入答案紙。

若你無法安裝 Matplotlib，也可以自行從 `results.csv` 計算中位數，並以任何工具繪製符合作業要求的 `performance.png`（需有座標名稱、時間單位與兩條曲線）。`results.csv` 仍須是 `benchmark.cpp` 直接輸出的原始 18 筆測量結果。

## 六、繳交前

1. 將 `answer_sheet.md` 轉成不超過 2 頁的 `answer_sheet.pdf`。
2. 在本檔開頭補上你實際使用的作業系統與編譯器版本。
3. 確認第二至五節的指令在乾淨的目錄中可以從頭重現結果。
4. 按作業公告的檔案清單建立 `studentID_HW1.zip`。
5. 教師提供的 `public_tests.cpp`、`naive_shift_queue.hpp`、`benchmark.cpp` 與 `plot_results.py` 不需放入繳交壓縮檔。
