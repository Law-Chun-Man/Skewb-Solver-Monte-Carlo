document.addEventListener('DOMContentLoaded', function () {
    const themeToggleButton = document.getElementById('theme-toggle');
    const body = document.body;

    // Check stored theme preference or default to light theme
    if (localStorage.getItem('theme') === 'dark') {
        body.classList.add('dark-mode');
        //themeToggleButton.textContent = 'Light Mode';
    } else {
        body.classList.remove('dark-mode');
        //themeToggleButton.textContent = 'Dark Mode';
    }

    themeToggleButton.addEventListener('click', function () {
        if (body.classList.contains('dark-mode')) {
            body.classList.remove('dark-mode');
            //themeToggleButton.textContent = 'Dark Mode';
            localStorage.setItem('theme', 'light');
        } else {
            body.classList.add('dark-mode');
            //themeToggleButton.textContent = 'Light Mode';
            localStorage.setItem('theme', 'dark');
        }
    });
});
