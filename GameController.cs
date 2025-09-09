using UnityEngine;
using System.Collections;

public class GameController : MonoBehaviour
{
    [Header("Refs")]
    public EnemySpawner spawner;
    public Transform enemiesContainer;
    public GameObject playerPrefab;
    public Transform playerSpawn;
    public GameObject tryAgainPanel;
    public GameObject youWinPanel;
    public GameObject youLosePanel; 

    bool diedThisLevel = false;
    GameObject currentPlayer;

    void OnEnable()
    {
        if (spawner != null)
            spawner.onSequenceCleared += OnLevelCleared;
    }

    void OnDisable()
    {
        if (spawner != null)
            spawner.onSequenceCleared -= OnLevelCleared;
    }

    void Start()
    {
        
        SpawnPlayer();
        HideAllOverlays(); 
      
    }

    void SpawnPlayer()
    {
        currentPlayer = Instantiate(
            playerPrefab,
            playerSpawn.position,
            Quaternion.Euler(0, 0, 90)   // Playership 
        );

        var relay = currentPlayer.GetComponent<PlayerDeathRelay>();
        if (relay == null) relay = currentPlayer.AddComponent<PlayerDeathRelay>();
        relay.onDied = OnPlayerDied;
        var health = currentPlayer.GetComponent<PlayerHealth>();
        if (health != null)
        {
            health.UIManager = Object.FindFirstObjectByType<PlayerLives>();
            Debug.Log("[GC]: Bound UIManager? " + (health.UIManager != null));
        }
    }

    void OnPlayerDied()
    {
        diedThisLevel = true;
        var lives = Object.FindFirstObjectByType<PlayerLives>(); 
        if (lives != null && lives.RemainingLives <= 0)
        {
            ShowLoseFinal();
            return; 
        }
        ShowTryAgain(); 
        StartCoroutine(RestartLevel());
    }

    IEnumerator RestartLevel()
    {
        Debug.Log("[GC] Restarting level from formation 1...");
        yield return new WaitForSeconds(1f);

        // 1. Clear all enemies safely (only EnemyShip, not spawner/container)
        if (enemiesContainer != null)
        {
            for (int i = enemiesContainer.childCount - 1; i >= 0; i--)
            {
                Transform child = enemiesContainer.GetChild(i);
                if (child != null && child.GetComponent<EnemyShip>() != null)
                {
                    Destroy(child.gameObject);
                }
            }
        }

        // 2. Clear bullets
        foreach (var bullet in GameObject.FindGameObjectsWithTag("EnemyBullet"))
            Destroy(bullet);

        // 3. Restart spawner full sequence
        if (spawner != null)
        {
            spawner.StopAllCoroutines();
            yield return null;   // wait 1 frame for destroy animations
            spawner.RunFromStart();   // run full sequence Line → V → Wave
        }

        // 4. Respawn player
        if (currentPlayer == null)
            SpawnPlayer();

        diedThisLevel = false;
        HideAllOverlays(); 
    }

    void OnLevelCleared()
    {
        if (!diedThisLevel)
        {
            var lives = Object.FindFirstObjectByType<PlayerLives>();
            if (lives == null || lives.RemainingLives > 0)
            {
                ShowWin(); 
            }
        }
        else
        {
            StartCoroutine(RestartLevel());
        }
    }
    void HideAllOverlays()
    {
        if (tryAgainPanel) tryAgainPanel.SetActive(false); 
        if (youWinPanel) youWinPanel.SetActive(false);
        if (youLosePanel) youLosePanel.SetActive(false);
    }
    void ShowTryAgain()
    {
        if (tryAgainPanel)
        {
            tryAgainPanel.SetActive(true);
            if (youWinPanel) youWinPanel.SetActive(false);
            if (youLosePanel) youLosePanel.SetActive(false); 
        }
    }
    void ShowWin()
    {
        if(youWinPanel)
        {
            youWinPanel.SetActive(true);
            if (tryAgainPanel) tryAgainPanel.SetActive(false);
            if (youLosePanel) youLosePanel.SetActive(false);
        }
    }
    public void ShowLoseFinal()
    {
        youLosePanel.SetActive(true);
        if (tryAgainPanel) tryAgainPanel.SetActive(false);
        if (youWinPanel) youWinPanel.SetActive(false); 
    }

}
