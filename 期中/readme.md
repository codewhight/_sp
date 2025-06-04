# 期中專案 thread 爬蟲
---
此作業修改上學期python 的期末專案 


在原先的程式基礎上加 thread 讓後端同時處理多個請求或背景任務
```
threading.Thread(target=self._fetch_finance_news_thread, daemon=True).start()

```

## 程式解釋
### 建置前端介面

  此處由chatgpt 協助完成

新聞顯示頁面

  透過點擊 "抓取新聞" 或 "抓取財金新聞" 來查看新聞
```
# 新聞顯示頁面
        self.news_frame = ttk.Frame(self.notebook)
        self.notebook.add(self.news_frame, text="最新新聞")
        
        self.news_text = tk.Text(
            self.news_frame,
            height=10,
            wrap=tk.WORD,
            font=('微軟正黑體', 10)
        )
        self.news_text.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)


        scrollbar = ttk.Scrollbar(self.news_frame, command=self.news_text.yview)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        self.news_text.config(yscrollcommand=scrollbar.set)

        
        self.button_frame = ttk.Frame(self.news_frame)
        self.button_frame.pack(side=tk.BOTTOM, pady=10)

        fetch_btn1 = ttk.Button(self.button_frame, text="抓取新聞", command=self.fetch_and_display_news)
        fetch_btn1.pack(side=tk.LEFT, padx=5, pady=10)

        fetch_btn2 = ttk.Button(self.button_frame, text="抓取財金新聞", command=self.fetch_and_display_finance_news)
        fetch_btn2.pack(side=tk.LEFT, padx=5, pady=10)

        self.fortune_frame = ttk.Frame(self.notebook)
```
星座運勢頁面

  透過選取下拉式選單 查看星座運勢
```
        # 星座運勢頁面
        self.notebook.add(self.fortune_frame, text="星座運勢")
        self.fortune_label = ttk.Label(self.fortune_frame, text="請選擇星座：")

        ttk.Label(self.fortune_frame, text="請選擇你的星座：", font=("Arial", 14)).pack(pady=10)
        self.zodiac_var = tk.StringVar()
        self.zodiac_combo = ttk.Combobox(self.fortune_frame, textvariable=self.zodiac_var, state="readonly",
                                         values=list(self.zodiac_dict.keys()))
        self.zodiac_combo.pack(pady=5)
        self.zodiac_combo.bind("<<ComboboxSelected>>", self.on_zodiac_selected)
        
        self.fortune_text = tk.Text(self.fortune_frame, wrap=tk.WORD, font=("Arial", 12))
        self.fortune_text.pack(expand=True, fill='both', padx=10, pady=10)
```
天氣預報頁面

  透過選取下拉式選單 查看天氣運勢
```
        #天氣預報頁面
        self.weather_frame = ttk.Frame(self.notebook)

        # 加入分頁
        self.notebook.add(self.weather_frame, text="天氣預報")

        # 建立 Label
        weather_label = ttk.Label(self.weather_frame, text="選擇縣市：")
        weather_label.pack(pady=5)

        # 建立變數來綁定選擇城市（StringVar）
        self.city_var = tk.StringVar(value="台北市")

        # 建立下拉選單
        city_dropdown = ttk.Combobox(
            self.weather_frame,
            textvariable=self.city_var,
            state="readonly",
            values=list(self.city_dist.keys())
        )
        city_dropdown.pack(pady=5)
        city_dropdown.bind("<<ComboboxSelected>>", lambda e: self.fetch_weather_thread())

        # 顯示天氣資訊的 Text 區塊
        self.weather_text = tk.Text(self.weather_frame, wrap=tk.WORD, height=10, width=50)
        self.weather_text.pack(padx=10, pady=10)
```
### 一般新聞爬蟲

  此處為原創 
  
  主要功能: 抓取新聞內容 顯示現在時間 顯示抓取到的新聞資料
```
 def _fetch_news_thread(self):
        url = "https://tw.news.yahoo.com/archive/"
        
        try:
            response = requests.get(url)
            if response.status_code == 200:
                soup = BeautifulSoup(response.text, 'html.parser')
                links = soup.find_all('div', class_='Ov(h) Pend(44px) Pstart(25px)')
                links = links[:10]

                results = [] 
                lock = threading.Lock()

                def fetch_one_news(i, a_tag):
                    if not a_tag:
                        return
                    title = a_tag.get_text(strip=True)
                    href = a_tag['href']
                    news_url = f"https://tw.news.yahoo.com{href}"
                    try:
                        news_response = requests.get(news_url)
                        news_soup = BeautifulSoup(news_response.text, 'html.parser')
                        content_tag = news_soup.find('div', class_='caas-body')
                        content = content_tag.get_text(strip=True) if content_tag else "無內文"
                        with lock:
                            results.append((i, title, content))
                    except:
                        with lock:
                            results.append((i, title, "【內文載入失敗】"))

                
                threads = []
                for i, div in enumerate(links, 1):
                    a_tag = div.find('a')
                    t = threading.Thread(target=fetch_one_news, args=(i, a_tag))
                    t.start()
                    threads.append(t)

                for t in threads:
                    t.join()

                results.sort(key=lambda x: x[0])  # 按編號排序

                # 主執行緒更新 UI
                def update_text():
                    self.news_text.delete(1.0, tk.END)
                    now_str = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                    self.news_text.insert(tk.END, f"現在時間：{now_str}\n\n")
                    for i, title, content in results:
                        self.news_text.insert(tk.END, f"{i}. {title}\n\n{content}\n\n{'-'*80}\n\n")

                self.news_text.after(0, update_text)

        except requests.RequestException:
            self.news_text.delete(1.0, tk.END)
            self.news_text.insert(tk.END, "無法取得新聞資料，請檢查網路連線。")
```
### 財金新聞爬蟲
  主要功能: 抓取財金新聞內容 顯示現在時間 顯示抓取到的財金新聞資料
```
def _fetch_finance_news_thread(self):
        url = "https://tw.stock.yahoo.com/"
        
        try:
            response = requests.get(url)
            if response.status_code == 200:
                soup = BeautifulSoup(response.text, 'html.parser')
                links = soup.find_all('div', class_='Ov(h) Pend(44px) Pstart(25px)')
                print(len(links))
                links = links[:10]
                results = [] 
                lock = threading.Lock()

                def fetch_one_news(i, a_tag):
                    if not a_tag:
                        return
                    title = a_tag.get_text(strip=True)
                    href = a_tag['href']
                    news_url = f"https://tw.news.yahoo.com/{href}"
                    print(href)
                    try:
                        news_response = requests.get(news_url)
                        news_soup = BeautifulSoup(news_response.text, 'html.parser')
                        content_tag = news_soup.find('div', class_='caas-body')
                        content = content_tag.get_text(strip=True) if content_tag else "無內文"
                        with lock:
                            results.append((i, title, content))
                    except:
                        with lock:
                            results.append((i, title, "【內文載入失敗】"))

                
                threads = []
                for i, div in enumerate(links, 1):
                    a_tag = div.find('a')
                    t = threading.Thread(target=fetch_one_news, args=(i, a_tag))
                    t.start()
                    threads.append(t)

                for t in threads:
                    t.join()

                results.sort(key=lambda x: x[0])  # 按編號排序

                # 主執行緒更新 UI
                def update_text():
                    self.news_text.delete(1.0, tk.END)
                    now_str = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                    self.news_text.insert(tk.END, f"現在時間：{now_str}\n\n")
                    for i, title, content in results:
                        self.news_text.insert(tk.END, f"{i}. {title}\n\n{content}\n\n{'-'*80}\n\n")

                self.news_text.after(0, update_text)

        except requests.RequestException:
            self.news_text.delete(1.0, tk.END)
            self.news_text.insert(tk.END, "無法取得新聞資料，請檢查網路連線。")
```
### 星座運勢爬蟲

  此處為原創
  
  主要功能: 抓取星座網頁內容 
```
 def show_fortune(self, zodiac):
        def fetch():
            zodiac_number = self.zodiac_dict.get(zodiac, -1)
            if zodiac_number == -1:
                self.fortune_text.after(0, lambda: self.fortune_text.insert(tk.END, "無效的星座，請重試。"))
                return

            url = f"https://astro.click108.com.tw/daily_{zodiac_number}.php?iAstro={zodiac_number}"
            try:
                response = requests.get(url)
                soup = BeautifulSoup(response.text, 'html.parser')
                fortune = soup.find('div', class_='TODAY_CONTENT').get_text()

                def update_text():
                    self.fortune_text.delete(1.0, tk.END)
                    self.fortune_text.insert(tk.END, fortune.strip())
                    self.notebook.select(1)  

                self.fortune_text.after(0, update_text)

            except requests.RequestException:
                self.fortune_text.after(0, lambda: self.fortune_text.insert(tk.END, "無法取得運勢資訊，請檢查網路連線。"))

        threading.Thread(target=fetch, daemon=True).start()

```

### 天氣爬蟲

  此處為原創
  
  主要功能: 抓取天氣網站資訊
```
def _fetch_weather(self):
    
        city = self.city_var.get()
        print(f"選擇的縣市：{city}")
        
        city_id = self.city_dist.get(city)
        print(f"對應的 city_id：{city_id}")
        if not city_id:
            self.weather_text.after(0, lambda: self.weather_text.insert(tk.END, "無效的縣市，請重試。"))
            return
        api_key = "08b6fd2de350969a1e7ee1b1fb5a1125"
        url = f"http://api.openweathermap.org/data/2.5/weather?q={city_id}&appid=08b6fd2de350969a1e7ee1b1fb5a1125&units=metric&lang=zh_tw"

        try:
            response = requests.get(url, timeout=10)
            if response.status_code == 200:

                response = requests.get(url, timeout=10)
                response.raise_for_status()
                data = response.json()
                
                temp = data['main']['temp']
                weather = data['weather'][0]['description']
                advice = "記得天冷加衣。" if temp < 15 else "今天天氣溫暖，適合外出。"
                advice += "下雨記得帶傘。" if "雨" in weather else ""
                
                weather_info = f"現在的天氣是{weather}，氣溫是{temp}°C。{advice}"
                   
                

                # 更新 UI 要透過 after
                def update_text():
                    self.weather_text.delete(1.0, tk.END)
                    self.weather_text.insert(tk.END, f"{city}：\n{weather}\n{weather_info}")

                self.weather_text.after(0, update_text)

            else:
                self.weather_text.after(0, lambda: self.weather_text.insert(tk.END, "無法取得天氣資料，請稍後再試。"))
        except requests.RequestException:
            self.weather_text.after(0, lambda: self.weather_text.insert(tk.END, "無法取得天氣資料，請檢查網路連線。"))

```


