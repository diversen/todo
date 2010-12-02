/*
http://papermashup.com/drag-drop-with-php-jquery/
*/

$(document).ready(function(){
    function slideout(){
        setTimeout(function(){
            $("#response").slideUp("slow", function () {
            });
        }, 2000);
    }
    $("#response").hide();
        $(function() {
            $("#list ul").sortable({
                opacity: 0.8, cursor: 'move', update: function() {
                    var order = $(this).sortable("serialize") + '&update=update';
                    $.post("/todo/update", order, function(theResponse){
                        $("#response").html(theResponse);
                        $("#response").slideDown('slow');
                    });
                }
            });
        });

    });