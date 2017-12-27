jQuery(document).ready(function() {
	function hide_all_team_memberselects() {
		jQuery('._roparun_teammember_select').parent().hide();
	}
	
	jQuery('.webform-component--donatie--team---roparun-team select').on('change', function() {
		var team_id = jQuery('.webform-component--donatie--team---roparun-team select').val();
		hide_all_team_memberselects();
		if (team_id) {
			var selector = '.form-item-submitted-donatie-team--roparun-team-member-'+team_id;
			jQuery(selector).show();
		}
			
	});
	
	jQuery('.webform-component--donatie--team---roparun-team select').trigger('change');
	
});