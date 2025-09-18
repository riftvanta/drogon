// Ensure Western numerals (0-9) are displayed
function ensureWesternNumerals() {
    const arabicNumerals = ['٠', '١', '٢', '٣', '٤', '٥', '٦', '٧', '٨', '٩'];
    const westernNumerals = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9'];

    function replaceNumerals(text) {
        for (let i = 0; i < arabicNumerals.length; i++) {
            text = text.replace(new RegExp(arabicNumerals[i], 'g'), westernNumerals[i]);
        }
        return text;
    }

    // Apply to all text nodes
    document.querySelectorAll('*').forEach(element => {
        if (element.childNodes.length > 0) {
            element.childNodes.forEach(node => {
                if (node.nodeType === Node.TEXT_NODE && node.textContent) {
                    node.textContent = replaceNumerals(node.textContent);
                }
            });
        }
    });
}

// Format date in Gregorian calendar with Western numerals
function formatGregorianDate() {
    const now = new Date();

    // Add 3 hours for Amman timezone (UTC+3)
    const ammanTime = new Date(now.getTime() + (3 * 60 * 60 * 1000));

    const year = ammanTime.getUTCFullYear();
    const month = String(ammanTime.getUTCMonth() + 1).padStart(2, '0');
    const day = String(ammanTime.getUTCDate()).padStart(2, '0');

    const dateStr = `${year}-${month}-${day}`;

    const dateElement = document.getElementById('current-date');
    if (dateElement) {
        dateElement.textContent = dateStr;
    }
}

// Initialize on page load
document.addEventListener('DOMContentLoaded', function() {
    // Format date
    formatGregorianDate();

    // Ensure Western numerals
    ensureWesternNumerals();

    // Update date every minute
    setInterval(formatGregorianDate, 60000);

    // Monitor for dynamic content changes
    const observer = new MutationObserver(function(mutations) {
        ensureWesternNumerals();
    });

    observer.observe(document.body, {
        childList: true,
        subtree: true,
        characterData: true
    });
});

// Handle form submission with validation
const loginForm = document.querySelector('.login-form');
if (loginForm) {
    loginForm.addEventListener('submit', function(e) {
        const username = document.getElementById('username').value.trim();
        const password = document.getElementById('password').value;

        if (!username || !password) {
            e.preventDefault();
            alert('الرجاء إدخال اسم المستخدم وكلمة المرور');
        }
    });
}

// Add hover effects for action buttons
document.querySelectorAll('.action-button:not(:disabled)').forEach(button => {
    button.addEventListener('click', function() {
        if (!this.disabled) {
            // Future functionality can be added here
            console.log('Action:', this.textContent.trim());
        }
    });
});

// Ensure RTL layout is properly applied
document.documentElement.setAttribute('dir', 'rtl');
document.documentElement.setAttribute('lang', 'ar');