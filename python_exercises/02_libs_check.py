# 演示 numpy / pandas / matplotlib 三个库能否正常使用
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# numpy：创建数组
arr = np.array([1, 2, 3, 4, 5])
print("numpy 数组:", arr, "平均值:", arr.mean())

# pandas：简单表格
df = pd.DataFrame({"姓名": ["小明", "小红"], "分数": [88, 95]})
print("\npandas 表格:")
print(df)

# matplotlib：画一条简单折线
plt.plot([1, 2, 3, 4], [1, 4, 9, 16])
plt.title("示例折线图")
# Windows 下 plt.show() 会弹出窗口，练习时可注释掉避免阻塞
# plt.show()
print("\nmatplotlib 已可用")
