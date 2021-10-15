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

  	if(this.projname.value.length!=0)
    {

  		this.alerts.setMessage(val,'success');
      var proj = JSON.parse(localStorage.getItem('projects'))

      var resultArray = Object.keys(proj).map(function(pr){
          let pro = proj[pr];
    return pro;
      });

      var currentId: number = Number(localStorage.getItem('nextId'));
      resultArray.push({"id":currentId,"name":this.projname.value,"location":this.projloc.value, "sensors":0})
      localStorage.setItem('nextId',String(currentId+1))
      localStorage.setItem('projects',JSON.stringify(resultArray))
    }
    this.closeModal();    
    window.location.reload();
  }

  closeModal() {
    this.dialogRef.close();

  }

}
