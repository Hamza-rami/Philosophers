
$(function() 
{
    $('#backLink').attr("href", (new URLSearchParams(window.location.search)).get('returnPath'));
});
