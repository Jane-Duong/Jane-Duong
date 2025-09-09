using UnityEngine;
using UnityEngine.UI;

public class PlayerLives : MonoBehaviour
{
    public int lives = 3;
    public Image[] livesUI;
    

    public void LoseLife()
    {
        lives--;
        Debug.Log("UIManager: lives = " + lives); 
        // Update UI
        for (int i = 0; i < livesUI.Length; i++)
            livesUI[i].enabled = (i < lives);

        if (lives <= 0)
        {
            Debug.Log("GAME OVER");
        
        }
    }
    public int RemainingLives
    {
        get
        {
            return lives; 
        }
    }
}
