# LSAP HW6

資管四 江彥宏 B11705044

---

# 1. Initial Setup and Repository Link

**GitHub Repository:** [https://github.com/cyh032508/lsap-cicd-example-app](https://github.com/cyh032508/lsap-cicd-example-app)

## Screenshot (ChatOps)

當 build 錯誤的時候會通過 Discord 機器人自動通知。

![截圖 2025-12-09 01.33.34.png](LSAP%20HW6/%E6%88%AA%E5%9C%96_2025-12-09_01.33.34.png)

---

# 2. CI/CD Demonstration Video YouTube Link

**YouTube Link:** [https://youtu.be/XVb77ODz6pE](https://youtu.be/XVb77ODz6pE)

## Phase 1: Development and Staging Deployment

Git push 上去之後他就會自動 build 了，執行 `docker ps` 可以看到 dev-app 有成功運行。

![截圖 2025-12-09 01.26.34.png](LSAP%20HW6/%E6%88%AA%E5%9C%96_2025-12-09_01.26.34.png)

## Phase 2: GitOps Promotion to Production

前一階段確認沒問題就切換到 main 並把剛剛 dev 的內容合併起來，接著打開 deploy.config 修改成要部署的版本號，接著 push 到 github 並且自動檢查、部署，最後成功運行在 docker 上，並且內容已經修改成新的了。

![截圖 2025-12-09 01.27.56.png](LSAP%20HW6/%E6%88%AA%E5%9C%96_2025-12-09_01.27.56.png)

## Phase 3: Rollback

接著 revert 成上個版本，再去查看 `docker ps` 可以看到有重新部署，並且在 revert 前我們去 `curl [localhost:8082](http://localhost:8082)` 得到的內容是 <Hello>，但當我們 revert 之後再去取用一次內容就變成了 <Welcome to the CI/CD Workshop!>。

![截圖 2025-12-09 01.13.54.png](LSAP%20HW6/%E6%88%AA%E5%9C%96_2025-12-09_01.13.54.png)

---

# 3. Docker Hub Bonus

在 dev 這個 branch 時修改 package.json 的版本號可以正確識別出並且貼上 tag。

![截圖 2025-12-09 01.39.08.png](LSAP%20HW6/%E6%88%AA%E5%9C%96_2025-12-09_01.39.08.png)