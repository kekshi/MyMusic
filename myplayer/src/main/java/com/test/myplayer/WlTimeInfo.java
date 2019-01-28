package com.test.myplayer;

public class WlTimeInfo {
    private int currentTime;
    private int totalTime;

    public int getCurrentTime() {
        return currentTime;
    }

    public void setCurrentTime(int currentTime) {
        this.currentTime = currentTime;
    }

    public int getTotalTime() {
        return totalTime;
    }

    public void setTotalTime(int totalTime) {
        this.totalTime = totalTime;
    }

    @Override
    public String toString() {
        return "WlTimeInfo{" +
                "currentTime=" + currentTime +
                ", totalTime=" + totalTime +
                '}';
    }
}
