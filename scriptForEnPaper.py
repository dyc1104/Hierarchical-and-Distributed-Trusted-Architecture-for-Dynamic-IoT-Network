
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rcParams

# ==========================================
# 1. 全局字体设置：严格遵循 Times New Roman
# ==========================================
rcParams['font.family'] = 'serif'
rcParams['font.serif'] = ['Times New Roman']
rcParams['mathtext.fontset'] = 'stix'  # 保证坐标轴数字也是类似的衬线字体
rcParams['font.size'] = 11  # 全局基准字号


# ==========================================
# Figure 4: Sybil Attack & Malicious Detection (a) (b)
# 数据来源：单次拒绝时间.txt & 检测.txt
# ==========================================
def plot_figure_4_scatter():
    # 女巫攻击防护：单次拒绝时间 (199 次真实数据)
    sybil_points = [
        0.110079, 0.108964, 0.109834, 0.110565, 0.109207, 0.109638, 0.110105, 0.108923, 0.10941, 0.108608,
        0.108147, 0.109586, 0.110223, 0.110927, 0.108317, 0.108169, 0.108468, 0.108784, 0.108456, 0.108933,
        0.110073, 0.108011, 0.109907, 0.109332, 0.108849, 0.109636, 0.108371, 0.109295, 0.109744, 0.109899,
        0.109206, 0.109604, 0.109682, 0.110575, 0.108757, 0.109277, 0.108674, 0.109003, 0.10849, 0.108656,
        0.110273, 0.10963, 0.10989, 0.109112, 0.109225, 0.108444, 0.110017, 0.109432, 0.109335, 0.108797,
        0.108196, 0.109636, 0.109108, 0.109817, 0.107742, 0.108079, 0.109182, 0.108806, 0.10992, 0.110445,
        0.11058, 0.1103, 0.108315, 0.109308, 0.108271, 0.108339, 0.109883, 0.108407, 0.109997, 0.110473,
        0.10953, 0.109096, 0.109211, 0.109106, 0.109867, 0.111109, 0.110265, 0.10824, 0.108596, 0.10959,
        0.108367, 0.10929, 0.107975, 0.10833, 0.108799, 0.108343, 0.110616, 0.107649, 0.108469, 0.108194,
        0.107523, 0.109587, 0.1082, 0.110253, 0.109516, 0.110104, 0.110123, 0.109849, 0.110512, 0.10964,
        0.110743, 0.108022, 0.109152, 0.108585, 0.109179, 0.109835, 0.109757, 0.108347, 0.109445, 0.108859,
        0.108166, 0.109129, 0.109999, 0.108606, 0.108406, 0.109158, 0.110361, 0.108558, 0.110179, 0.109255,
        0.108054, 0.108035, 0.109549, 0.109006, 0.109292, 0.109393, 0.108465, 0.109072, 0.108531, 0.107943,
        0.109758, 0.110164, 0.108917, 0.109691, 0.108009, 0.108978, 0.109242, 0.109994, 0.109432, 0.110791,
        0.109347, 0.108768, 0.10958, 0.110747, 0.10878, 0.109152, 0.110271, 0.107787, 0.10841, 0.109038,
        0.1094, 0.107836, 0.108511, 0.11119, 0.10987, 0.109922, 0.109821, 0.109275, 0.109916, 0.109978,
        0.110044, 0.10948, 0.10977, 0.109455, 0.109867, 0.109211, 0.109727, 0.108768, 0.110544, 0.10804,
        0.108674, 0.109044, 0.109979, 0.109079, 0.110178, 0.109815, 0.109111, 0.108831, 0.10876, 0.109881,
        0.108213, 0.107861, 0.109025, 0.108903, 0.108854, 0.109722, 0.108161, 0.110381, 0.110855, 0.108144,
        0.108746, 0.109764, 0.108431, 0.109457, 0.109107, 0.108629, 0.109215, 0.108562, 0.108004
    ]

    # 恶意节点检测：识别与隔离延迟 (165 次真实数据)
    malicious_points = [
        0.0606793, 0.0594718, 0.0600762, 0.0587143, 0.0605669, 0.0613898, 0.059059, 0.0590567, 0.0614995,
        0.059773, 0.0596112, 0.0592241, 0.0611618, 0.0595922, 0.0600554, 0.0589753, 0.0615901, 0.0592265,
        0.0612967, 0.057833, 0.0598723, 0.0624909, 0.0584779, 0.0608007, 0.0583825, 0.0602234, 0.059933,
        0.062228, 0.0574481, 0.0601356, 0.0622486, 0.0601136, 0.0574599, 0.0611263, 0.0591668, 0.0597291,
        0.0605897, 0.0618193, 0.0587676, 0.0605158, 0.0596587, 0.0591328, 0.0614569, 0.0580126, 0.0620145,
        0.0590402, 0.0616081, 0.059656, 0.0586932, 0.059419, 0.0613735, 0.0599554, 0.0596988, 0.0601832,
        0.0589926, 0.0600459, 0.0602098, 0.0600401, 0.0609269, 0.0590021, 0.0607937, 0.0611348, 0.0588963,
        0.0594099, 0.0592508, 0.0613366, 0.0592244, 0.0594265, 0.062028, 0.0593469, 0.0586503, 0.061481,
        0.0597574, 0.0596948, 0.0607991, 0.0591082, 0.0592003, 0.0609274, 0.0615077, 0.0600034, 0.0567139,
        0.0615235, 0.0598947, 0.0605671, 0.0602116, 0.0589868, 0.0611744, 0.0588508, 0.0592618, 0.0607174,
        0.0604695, 0.059895, 0.059059, 0.0620639, 0.0606717, 0.0586244, 0.0593939, 0.0602924, 0.059964,
        0.0604421, 0.0581827, 0.0625479, 0.0579859, 0.0590636, 0.0613804, 0.0613474, 0.0600027, 0.0589322,
        0.0596934, 0.0592096, 0.0618012, 0.0588263, 0.0614615, 0.0604746, 0.0596705, 0.0573451, 0.0621844,
        0.0609083, 0.0594283, 0.0580471, 0.0611702, 0.0604402, 0.059944, 0.0588733, 0.062147, 0.0594483,
        0.0587093, 0.060298, 0.0595342, 0.0611555, 0.0594991, 0.0593661, 0.0606193, 0.0599483, 0.0598517,
        0.0600758, 0.0593622, 0.0604185, 0.0616825, 0.0569838, 0.0604494, 0.0613698, 0.0601736, 0.0599433,
        0.0586531, 0.061827, 0.0586616, 0.0611184, 0.0589447, 0.061358, 0.0603696, 0.0604297, 0.0594063,
        0.0598224, 0.0590779, 0.0597452, 0.0603544, 0.0594711, 0.0610166, 0.0589976, 0.0604692, 0.0603462,
        0.0625987, 0.0553012, 0.0617705
    ]

    # 计算均值
    avg_sybil = np.mean(sybil_points)
    avg_malicious = np.mean(malicious_points)

    # 创建 1行2列的画板
    fig, axes = plt.subplots(1, 2, figsize=(10, 5))
    fig.tight_layout(pad=4.0)

    # ==========================
    # (a) Sybil Attack
    # ==========================
    ax1 = axes[0]
    x_sybil = range(1, len(sybil_points) + 1)

    # 散点 (红色系，半透明)
    ax1.scatter(x_sybil, sybil_points, color='#79A3CD', alpha=0.7, s=20, label='Single Delay')
    # 均值虚线 (深色)
    ax1.axhline(y=avg_sybil, color='#B5DCEB', linewidth=2, label=f'Average: {avg_sybil:.3f} s')

    # 设置标签与样式
    ax1.set_title('(a) Sybil Attack Rejection Time', fontweight='bold', pad=15, fontsize=14)
    ax1.set_xlabel('Request Index', fontsize=12)
    ax1.set_ylabel('Latency (s)', fontsize=12)
    ax1.set_ylim(0, 0.15)  # 对齐参考图的 Y 轴范围

    # 图例位置和网格
    ax1.legend(loc='lower right', fontsize=10)
    ax1.grid(True, linestyle=':', alpha=0.6)

    # ==========================
    # (b) Malicious Detection
    # ==========================
    ax2 = axes[1]
    x_malicious = range(1, len(malicious_points) + 1)

    # 散点 (蓝色系，半透明)
    ax2.scatter(x_malicious, malicious_points, color='#60B9B7', alpha=0.7, s=20, label='Single Delay')
    # 均值虚线 (橙色)
    ax2.axhline(y=avg_malicious, color='#9AC6A5', linewidth=2, label=f'Average: {avg_malicious:.3f} s')

    # 设置标签与样式
    ax2.set_title('(b) Detection & Isolation Delay', fontweight='bold', pad=15, fontsize=14)
    ax2.set_xlabel('Event Index', fontsize=12)
    ax2.set_ylabel('Latency (s)', fontsize=12)
    ax2.set_ylim(0, 0.10)  # 对齐参考图的 Y 轴范围

    # 图例位置和网格
    ax2.legend(loc='lower right', fontsize=10)
    ax2.grid(True, linestyle=':', alpha=0.6)

    # 显示图像
    #plt.show()
    plt.savefig("ex_attack.pdf", dpi=300, bbox_inches='tight')
    plt.close()

# ==========================================
# Figure 1: End-to-End Latency & TPS (a)(b)
# ==========================================
def plot_figure_1():
    # X轴: 报告数量
    x_reports = [1000, 2000, 3000, 4000, 6000, 8000, 10000, 20000, 30000, 40000, 50000]

    # 时延数据 (秒)
    lat_centralized = [0.1845, 0.3253, 0.4876, 0.6384, 0.9904, 1.2039, 1.4892, 3.2458, 4.5047, 6.4726, 9.8633]
    lat_distributed = [0.0915, 0.1073, 0.1250, 0.1364, 0.2021, 0.2261, 0.2432, 0.4129, 0.5578, 0.7377, 0.9267]

    # TPS 数据
    tps_centralized = [5418.2, 6147.16, 6151.4, 6264.79, 6058.12, 6644.96, 6714.84, 6161.66, 6659.68, 6179.88, 5069.25]
    tps_distributed = [10927.03, 18637.53, 23998.70, 29316.02, 29683.69, 35369.20, 41104.99, 48427.07, 53776.07,
                       54221.55, 53954.19]

    fig, axes = plt.subplots(1, 2, figsize=(12, 5))
    fig.tight_layout(pad=4.0)

    # (a) Scalability: Total Verification Latency
    ax1 = axes[0]
    ax1.plot(x_reports, lat_centralized, marker='o', color='#60B9B7', linewidth=2, label='Centralized (Traditional)')
    ax1.plot(x_reports, lat_distributed, marker='s', color='#9AC6A5', linewidth=2, label='Proposed Distributed')
    ax1.set_title('(a) Scalability: Total Verification Latency', pad=15)
    ax1.set_xlabel('Number of Data Reports')
    ax1.set_ylabel('Total Verification Time (s)')
    ax1.legend(loc='upper left')
    ax1.grid(True, linestyle='--', alpha=0.6)

    # (b) System Throughput Comparison
    ax2 = axes[1]
    ax2.plot(x_reports, tps_centralized, marker='o', linestyle='--', color='#60B9B7', linewidth=2,
             label='Centralized (Traditional)')
    ax2.plot(x_reports, tps_distributed, marker='s', linestyle='-', color='#9AC6A5', linewidth=2,
             label='Proposed Distributed')
    ax2.set_title('(b) System Throughput Comparison', pad=15)
    ax2.set_xlabel('Number of Data Reports')
    ax2.set_ylabel('Throughput (TPS)')
    ax2.legend(loc='upper left')
    ax2.grid(True, linestyle='--', alpha=0.6)

    #plt.show()
    plt.savefig("ex_stu.pdf", dpi=300, bbox_inches='tight')
    plt.close()


# ==========================================
# Figure 2: Node Join & Leave Latency (a)(b)
# ==========================================
def plot_figure_2():
    fig, axes = plt.subplots(1, 2, figsize=(10, 5))
    fig.tight_layout(pad=4.0)

    # (a) Node Join Latency
    ax_a = axes[0]
    join_categories = ['Device', 'Peer', 'Centralized']
    join_latencies = [0.0962, 0.1281, 0.3235]

    bars_a = ax_a.bar(join_categories, join_latencies, color=['#60B9B7', '#93B7D9', '#9AC6A5'], width=0.3)
    ax_a.set_title('(a) Node Join Latency', pad=15)
    ax_a.set_ylabel('Latency (s)')
    ax_a.set_ylim(0, 0.4)
    ax_a.grid(axis='y', linestyle='--', alpha=0.6)

    for bar in bars_a:
        yval = bar.get_height()
        ax_a.text(bar.get_x() + bar.get_width() / 2, yval + 0.005, f"{yval:.4f}", ha='center', va='bottom')

    # (b) Node Leave Latency
    ax_b = axes[1]
    leave_scenarios = ['Voluntary', 'No Response', 'Forced']
    device_leave = [0.0752, 0.0719, 0.0894]
    peer_leave = [0.0757, 0.0550, 0.1038]
    cent_leave = [0.0240, 0.0112, 0.0235]

    x = np.arange(len(leave_scenarios))
    width = 0.28

    bars_b1 = ax_b.bar(x - width, device_leave, width, label='Device', color='#60B9B7')
    bars_b2 = ax_b.bar(x, peer_leave, width, label='Peer', color='#93B7D9')
    bars_b3 = ax_b.bar(x + width, cent_leave, width, label='Centralized', color='#9AC6A5')

    ax_b.set_title('(b) Node Leave Latency', pad=15)
    ax_b.set_ylabel('Latency (s)')
    ax_b.set_xticks(x)
    ax_b.set_xticklabels(leave_scenarios)
    ax_b.set_ylim(0, 0.13)
    ax_b.legend(loc='upper left')
    ax_b.grid(axis='y', linestyle='--', alpha=0.6)

    def add_grouped_labels(bars):
        for bar in bars:
            yval = bar.get_height()
            ax_b.text(bar.get_x() + bar.get_width() / 2, yval + 0.002, f"{yval:.4f}", ha='center', va='bottom',
                      fontsize=9)

    add_grouped_labels(bars_b1)
    add_grouped_labels(bars_b2)
    add_grouped_labels(bars_b3)

    #plt.show()
    plt.savefig("ex_dynamic.pdf", dpi=300, bbox_inches='tight')
    plt.close()

def plot_combined_latency_and_memory():
    # 创建 1行2列的画布
    fig, axes = plt.subplots(1, 2, figsize=(12, 5.5))
    fig.tight_layout(pad=5.0)

    # ==========================
    # (a) Cryptographic Latency (数据来自 piver.txt)
    # ==========================
    ax1 = axes[0]
    stages = ['Sign\nGenerate', 'Verify\nEach', 'Aggregate', 'Verify\nAggregated']
    times = [11.66, 14.61, 0.59, 2.92]
    colors = ['#5ab4ac', '#8cadd1', '#9bcfa6', '#add1df']

    bars = ax1.bar(stages, times, color=colors, width=0.45, edgecolor='none')

    ax1.set_title('(a) Cryptographic Operation Latency (N=1000)', pad=15)
    ax1.set_ylabel('Computation Time (s)')
    ax1.set_ylim(0, 20.0)
    ax1.grid(axis='y', linestyle='--', alpha=0.6)

    for bar in bars:
        yval = bar.get_height()
        ax1.text(bar.get_x() + bar.get_width() / 2, yval + 0.3, f"{yval:.2f} s",
                 ha='center', va='bottom', fontsize=11)

    # ==========================
    # (b) Memory Consumption (数据来自 pi_block.txt 和 piver.txt)
    # ==========================
    ax2 = axes[1]

    # 从 pi_block.txt 提取的 67 个真实内存采样点 (单位: MB)
    fabric_memory_real = [
        1289, 1289, 1290, 1290, 1290, 1290, 1290, 1290, 1290, 1290,
        1274, 1273, 1273, 1273, 1273, 1273, 1273, 1273, 1272, 1272,
        1287, 1288, 1277, 1277, 1269, 1283, 1269, 1279, 1283, 1281,
        1274, 1287, 1290, 1297, 1290, 1282, 1285, 1282, 1286, 1275,
        1281, 1269, 1278, 1286, 1288, 1284, 1283, 1287, 1293, 1291,
        1283, 1302, 1306, 1310, 1306, 1312, 1316, 1310, 1316, 1309,
        1308, 1304, 1320, 1305, 1327, 1335, 1341
    ]

    # 根据日志时间戳 (10:51:25 到 10:52:54)，总时长约为 89 秒
    # 生成对应的真实时间轴
    x_time_real = np.linspace(0, 89, len(fabric_memory_real))

    # Proposed Light Node 内存：基于 piver.txt 的验证阶段 max_rss_kb 3584KB (~3.5MB)
    proposed_memory_real = np.full(len(x_time_real), 3.5)

    # 绘制真实数据曲线
    ax2.plot(x_time_real, fabric_memory_real, color='#60B9B7', linewidth=2, label='Naive Blockchain (Fabric Node)')
    ax2.plot(x_time_real, proposed_memory_real, color='#9AC6A5', linewidth=2.5, linestyle='--',
             label='Proposed Light Node (DN)')

    # 开启对数坐标轴
    ax2.set_yscale('log')

    ax2.set_title('(b) Memory Consumption on IoT Device', pad=15)
    ax2.set_xlabel('Simulation Time (s)')
    ax2.set_ylabel('Memory Usage (MB)')

    # 设置 X 轴范围，让画面比例更舒展
    ax2.set_xlim(-5, 95)

    ax2.grid(True, which="both", linestyle='--', alpha=0.5)

    # 动态计算文字注释的 Y 轴位置，避免硬编码偏离真实曲线
    fabric_avg = np.mean(fabric_memory_real)
    ax2.text(5, fabric_avg * 0.75, f'Fabric: ~{int(fabric_avg)} MB', color='black', fontsize=11)
    ax2.text(5, 5, 'Proposed: ~3.5 MB', color='black', fontsize=11)

    ax2.legend(loc='center right', fontsize=11)

    # 显示图表
    #plt.show()
    plt.savefig("ex_algo.pdf", dpi=300, bbox_inches='tight')
    plt.close()

if __name__ == "__main__":
    # plot_figure_1()
    # plot_figure_2()
    plot_combined_latency_and_memory()
    # plot_figure_4_scatter()
