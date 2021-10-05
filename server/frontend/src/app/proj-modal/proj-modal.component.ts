import { Component, OnInit } from '@angular/core';
import { MatDialogRef } from '@angular/material/dialog';
import { Router } from '@angular/router';
import { FormControl } from '@angular/forms';
import { AlertsService } from 'angular-alert-module';

@Component({
  selector: 'app-proj-modal',
  templateUrl: './proj-modal.component.html',
  styleUrls: ['./proj-modal.component.css']
})
export class ProjModalComponent implements OnInit {

  constructor(public dialogRef: MatDialogRef<ProjModalComponent>, private route : Router, private alerts: AlertsService) { }

  projname =  new FormControl('');
  projloc= new FormControl('');
  ngOnInit(): void {
  }

  addProj() {

  	console.log(this.projname.value, this.projloc.value);
  	let val = this.projname.value + " added"
  	//var proj = localStorage.getItem('project')
  	//localStorage.setItem('project', this.projname.value);
    this.closeModal();
  	if(this.projname.value.length!=0)
    {

  		this.alerts.setMessage(val,'success');
      var proj = JSON.parse(localStorage.getItem('projects'))
      proj.append({"id":1000,"name":this.projname.value,"location":this.projloc.value})
      localStorage.setItem('projects',JSON.stringify(proj))
    }
    
  }

  closeModal() {
    this.dialogRef.close();

  }

}
