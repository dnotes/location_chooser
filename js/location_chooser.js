Drupal.behaviors.locationChooser = function(context) {
  $("#edit-location-chooser").change(function() {
    var newLocation = $(this).val();
    var newLocationData = Drupal.settings.location_chooser[newLocation];
    $.each(newLocationData, function(key, value) {
      $("#edit-locations-0-" + key).val(value);
    })
  })
  $("fieldset.location .form-item input").change(function() {
    $("#edit-location-chooser").val(Drupal.settings.location_chooser.def);
  })
}
